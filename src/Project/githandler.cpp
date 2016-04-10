#include "githandler.h"

#include <QApplication>
#include <QThread>

#include "git2.h"
#include "global.h"
#include "Merge/mergedialog.h"
#include "worker.h"
#include "cloneworker.h"
#include "pushworker.h"

#define CSTR(qstring) (qstring.toStdString().c_str())

#define EX_ASSERT( expr ) { int code = (expr); Q_ASSERT(code); Q_UNUSED(code); }
#define GIT_ASSERT( expr ) { int code = (expr); Q_ASSERT( code == 0 ); }

GitHandler::GitHandler() :
    m_thread(new QThread(this)),
    m_worker(nullptr)
{
    git_libgit2_init();
}

GitHandler::~GitHandler()
{
    m_thread->exit();
    m_thread->wait();
    Q_ASSERT(m_worker == nullptr);
    git_libgit2_shutdown();
}

void GitHandler::startClone(git_repository* &repository, const QString &url, const QString &path, const git_clone_options* options)
{
    m_abortFlag = false;
    Q_ASSERT(repository == nullptr);

    Q_ASSERT(m_worker == nullptr);
    m_worker = new CloneWorker(repository, url, path, options);
    m_worker->moveToThread(m_thread);
    connect(m_thread, SIGNAL(started()), m_worker, SLOT(run()));
    m_thread->start();
}

void GitHandler::startPush(git_repository *repository, git_remote* &remote, git_strarray* refspecs, const git_push_options* options)
{
    Q_ASSERT(repository);

    Q_ASSERT(remote == nullptr);
    git_remote_lookup( &remote, repository, "origin" );

    // do the push
    Q_ASSERT(m_worker == nullptr);
    m_worker = new PushWorker(remote, refspecs, options);
    m_worker->moveToThread(m_thread);
    connect(m_thread, SIGNAL(started()), m_worker, SLOT(run()));
    m_thread->start();
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

bool GitHandler::error() const
{
    return m_error;
}

bool GitHandler::isAborted() const
{
    return m_abortFlag;
}

bool GitHandler::isFinished() const
{
    return m_worker->isFinished();
}

void GitHandler::abort()
{
    m_abortFlag = true;
}

void GitHandler::killWorker()
{
    if (m_worker)
    {
        m_thread->exit();
        m_thread->wait();

        m_error = m_worker->error();
        delete m_worker;
        m_worker = nullptr;
    }
}


















