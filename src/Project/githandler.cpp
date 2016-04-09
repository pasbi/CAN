#include "githandler.h"
#include "git2.h"
#include "global.h"

#define EX_ASSERT( expr ) { int code = (expr); Q_ASSERT(code); Q_UNUSED(code); }
#define GIT_ASSERT( expr ) { int code = (expr); qDebug() << #expr << "terminated with code " << code; Q_ASSERT( code == 0 ); }

GitHandler::GitHandler()
{
    git_libgit2_init();
}

GitHandler::~GitHandler()
{
    git_libgit2_shutdown();
}

void GitHandler::setURL(const QString &url)
{
    m_url = url;
}

void GitHandler::setMasterFilename(const QString &filename)
{
    m_masterFilename = filename;
}

void GitHandler::setRemoteFilename(const QString &filename)
{
    m_remoteFilename = filename;
}

QString GitHandler::masterFilename() const
{
    if (QFileInfo(m_masterFilename).isAbsolute())
    {
        return m_masterFilename;
    }
    else
    {
        qWarning() << "master filename shall be absolute.";
        return m_masterFilename;
    }
}

QString GitHandler::remoteFilename(const QTemporaryDir& dir) const
{
    if (QFileInfo(m_remoteFilename).isAbsolute())
    {
        return QDir(dir.path()).absoluteFilePath(m_remoteFilename);
    }
    else
    {
        qWarning() << "remote filename shall be relative.";
        return m_remoteFilename;
    }
}

bool GitHandler::download(const QString& url, const QString& file, const QString& targetFilename, const QString& username, const QString& password)
{
    QTemporaryDir dir;
    git_repository* repository = nullptr;

    // clone the repo to a temp dir
    clone(repository, url, dir.path(), username, password);

    // try to delete target file if it exists.
    if (QFileInfo(targetFilename).exists())
    {
        if (!QFile(targetFilename).remove())
        {
            qWarning() << "cannot overwrite " << targetFilename;

            git_repository_free(repository);
            repository = nullptr;

            return false;
        }
    }

    // check if source file exists
    QString absoluteSourceFilepath = QDir(dir.path()).absoluteFilePath(file);
    QFileInfo sourceFileInfo(absoluteSourceFilepath);
    if (!sourceFileInfo.exists() || !sourceFileInfo.isFile())
    {
        qWarning() << absoluteSourceFilepath << "is not a file.";

        git_repository_free(repository);
        repository = nullptr;

        return false;
    }

    // copy the file
    if (!QFile::copy(absoluteSourceFilepath, targetFilename))
    {
        qWarning() << "cannot copy" << remoteFilename(dir) << "->" << masterFilename();
        return false;
    }

    return true;
}

#define CSTR(qstring) (qstring.toStdString().c_str())

int credential_cb(git_cred **out, const char *url, const char *username_from_url, unsigned int allowed_types, void *payload)
{
    Q_UNUSED( url );
    Q_UNUSED( username_from_url );
    Q_UNUSED( allowed_types );
    Q_UNUSED( payload );

    QString username = static_cast<const GitHandler::CredentialPayload*>(payload)->username;
    QString password = static_cast<const GitHandler::CredentialPayload*>(payload)->password;

    qDebug() << "use username and password: " << username << password;

    GIT_ASSERT( git_cred_userpass_plaintext_new( out, CSTR(username), CSTR(password) ) );
    return 0;
}

int transferProgress_cb(const git_transfer_progress *stats, void *payload)
{
    Q_UNUSED(payload);
    qDebug() << "receiverd bytes: " << stats->received_bytes;
    return 0;
}

bool GitHandler::clone(git_repository* &repository, const QString &url, const QString &path, const QString& username, const QString& password)
{
    qDebug() << "start clone... " << url << path << username << password;
    Q_ASSERT(repository == nullptr);

    CredentialPayload payload(username, password);

    git_clone_options options = GIT_CLONE_OPTIONS_INIT;
    options.fetch_opts.callbacks.credentials = credential_cb;
    options.fetch_opts.callbacks.payload = &payload;
    options.fetch_opts.callbacks.transfer_progress = transferProgress_cb;

    if (int error = git_clone( &repository, CSTR(url), CSTR(path), &options ))
    {
        qWarning() << "Clone error: " << error;
        return false;
    }

    return true;
}

bool GitHandler::push(git_repository *repository, const QString& username, const QString& password)
{
    Q_ASSERT(repository);

    git_remote* remote = nullptr;
    git_remote_lookup( &remote, repository, "origin" );

    //setup callbacks
    git_remote_callbacks callbacks = GIT_REMOTE_CALLBACKS_INIT;
    callbacks.credentials = credential_cb;
    CredentialPayload payload(username, password);
    callbacks.payload = &payload;
    callbacks.transfer_progress = &transferProgress_cb;

    // setup options
    git_push_options options;
    GIT_ASSERT( git_push_init_options( &options, GIT_PUSH_OPTIONS_VERSION ) );
    options.callbacks = callbacks;

    // setup refspecs
    git_strarray refspecs;
    refspecs.count = 1;
    QString qrefspec("refs/heads/master:refs/heads/master");
    char* refspec = new char[qrefspec.length()];
    strcpy(refspec, qrefspec.toStdString().c_str());
    refspecs.strings = &refspec;

//    delete[] refspec;
//    refspec = nullptr;
//    refspecs.strings = nullptr;
//    refspecs.count = 0;

    // do the push
    GIT_ASSERT(git_remote_push( remote, &refspecs, &options));

    git_remote_free(remote);
    remote = nullptr;

    return true;
}

bool GitHandler::commit(git_repository* repo, const QString& filename, const QString& author, const QString& email, const QString& message)
{
    // create tree
    //create a tree from m_index
    git_tree* tree = nullptr;
    git_oid tree_id;
    git_index* index = nullptr;

    GIT_ASSERT(git_repository_index( &index, repo ));

    if (QFileInfo(filename).isAbsolute())
    {
        qFatal("filename must be relative to repository root.");
    }
    GIT_ASSERT(git_index_add_bypath(index, CSTR(filename)));
    GIT_ASSERT(git_index_write(index));

    GIT_ASSERT(git_index_write_tree(&tree_id, index));
    GIT_ASSERT(git_tree_lookup(&tree, repo, &tree_id));


    git_signature *sig = nullptr;
    GIT_ASSERT(git_signature_now(&sig, CSTR(author), CSTR(email)));

    // look up parent
    git_commit * parentCommit = nullptr;
    git_oid oid_parent_commit;
    GIT_ASSERT(git_reference_name_to_id(&oid_parent_commit, repo, "HEAD"));
    GIT_ASSERT(git_commit_lookup( &parentCommit, repo, &oid_parent_commit));
    Q_ASSERT(parentCommit);
    const git_commit *parentCommits[] = {parentCommit};

    git_oid new_commit_id;
    GIT_ASSERT(git_commit_create(   &new_commit_id,
                                    repo,
                                    "HEAD",                      /* name of ref to update */
                                    sig,                         /* author */
                                    sig,                         /* committer */
                                    "UTF-8",                     /* message encoding */
                                    CSTR(message),               /* message */
                                    tree,                        /* root tree */
                                    1,                           /* parent count */
                                    parentCommits                /* parents */   ) );




    git_signature_free(sig);
    git_commit_free(parentCommit);
    git_tree_free(tree);

    return true;
}







































// I think we'll never need pull.

//bool GitHandler::pull(git_repository* repository)
//{
//    Q_ASSERT(repository);

//    // fetch...
//    git_remote* remote = nullptr;
//    git_remote_lookup( &remote, repository, "origin" );
//    Q_ASSERT( remote );

//    Q_ASSERT(git_remote_add_fetch(remote, "+refs/heads/*:refs/remotes/origin/*") == 0);
//    Q_ASSERT(git_remote_fetch( remote, nullptr, nullptr, nullptr ) == 0);


//    // merge fetch head
//    git_oid fetch_head_oid;
//    Q_ASSERT(git_reference_name_to_id( &fetch_head_oid, m_repository, "FETCH_HEAD" ) == 0);

//    git_annotated_commit* their_head = nullptr;
//    const char* url = git_remote_url( remote );
//    Q_ASSERT(git_annotated_commit_from_fetchhead( &their_head, m_repository, "master", url, &fetch_head_oid ) == 0);

//    git_merge_options merge_options;
//    merge_options.version = GIT_MERGE_OPTIONS_VERSION;
//    merge_options.file_favor = GIT_MERGE_FILE_FAVOR_NORMAL;
//    merge_options.flags = GIT_MERGE_TREE_FIND_RENAMES;
//    merge_options.metric = nullptr;    // Default metric
//    merge_options.rename_threshold = 50;
//    merge_options.target_limit = 200;

//    git_checkout_options checkout_options;
//    assert( CHECK_GIT( git_checkout_init_options( &checkout_options, GIT_CHECKOUT_OPTIONS_VERSION ) ) );
//    checkout_options.checkout_strategy = GIT_CHECKOUT_FORCE;

//    // this call succeedes, even if there are conflicts. If it fails, something went very wrong and there is no chance for this to become consistent.
//    assert( CHECK_GIT( git_merge( m_repository, (const git_annotated_commit**) &their_head, 1, &merge_options, &checkout_options ) ) );

//    git_repository_state_cleanup( m_repository );
//    git_remote_free( remote );
//    remote = nullptr;

//    return true;
//}
