#include "githandler.h"
#include "git2.h"
#include "global.h"

#define EX_ASSERT( expr ) { int expressionErrorCode = (expr); Q_ASSERT(expressionErrorCode); Q_UNUSED(expressionErrorCode); }

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

bool GitHandler::download()
{
    QTemporaryDir dir;
    git_repository* repository = nullptr;

    clone(repository, m_url, dir.path());

    if (QFileInfo(m_masterFilename).exists())
    {
        if (!QFile(m_masterFilename).remove())
        {
            qWarning() << "cannot overwrite " << m_masterFilename;
            return false;
        }
    }

    if (!QFile(remoteFilename(dir)).copy(masterFilename()))
    {
        qWarning() << "cannot copy" << remoteFilename(dir) << "to" << masterFilename();
        return false;
    }

    return true;
}

#define CSTR(qstring) (qstring.toStdString().c_str())

bool GitHandler::clone(git_repository* &repository, const QString &url, const QString &path)
{
    Q_ASSERT(repository == nullptr);

    if (int error = git_clone( &repository, CSTR(url), CSTR(path), nullptr ))
    {
        qWarning() << "Clone error: " << error;
        return false;
    }

    return true;
}

int credential_cb( git_cred **out,
                   const char *url,
                   const char *username_from_url,
                   unsigned int allowed_types,
                   void *payload                    )
{
    Q_UNUSED( url );
    Q_UNUSED( username_from_url );
    Q_UNUSED( allowed_types );

    QString loginName =  static_cast<const GitHandler::CredentialPayload*>(payload)->username;
    QString password  =  static_cast<const GitHandler::CredentialPayload*>(payload)->password;

    EX_ASSERT(git_cred_userpass_plaintext_new( out, CSTR(loginName), CSTR(password) ) == 0);

    return true;
}

bool GitHandler::push(git_repository *repository, const QString& username, const QString& password)
{
    Q_ASSERT(repository);

    git_remote_callbacks callbacks = GIT_REMOTE_CALLBACKS_INIT;
    callbacks.credentials = credential_cb;

    CredentialPayload payload(username, password);

    callbacks.payload = &payload;

    git_remote* remote = nullptr;
    git_remote_lookup( &remote, repository, "origin" );

    EX_ASSERT(git_remote_connect( remote, GIT_DIRECTION_PUSH, &callbacks, nullptr) == 0);

    if (!git_remote_connected( remote ))
    {
        qWarning() << "connection not established.";

        git_remote_free(remote);
        remote = nullptr;

        return false;
    }

    git_push_options options;
    EX_ASSERT( git_push_init_options( &options, GIT_PUSH_OPTIONS_VERSION ) );

                           //"+refs/heads/*:refs/remotes/origin/*"
                           //"refs/heads/master:refs/heads/master"
    EX_ASSERT( git_remote_add_push( repository, "origin",  "refs/heads/master:refs/heads/master" ) );

    if (int error = git_remote_upload( remote, nullptr, &options ))
    {
        qWarning() << "Pushing failed: " << error;

        git_remote_free(remote);
        remote = nullptr;

        return false;
    }
    else
    {
        git_remote_free(remote);
        remote = nullptr;

        return true;
    }

}

bool GitHandler::commit()
{

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
