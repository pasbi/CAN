#ifndef GITHANDLER_H
#define GITHANDLER_H

#include <QTemporaryDir>
#include "git2.h" //TODO this should be implementation detail

class Project;
class Worker;

/**
 * @brief The GitHandler class handles a remote git repository.
 *  download clones the repository to a temporary folder.
 */
class GitHandler : public QObject
{
    Q_OBJECT

public:
    enum Error { CannotConnect, User };

    GitHandler();
    virtual ~GitHandler();
    bool error() const;
    void abortPush(git_remote* remote);

public slots:
    void requestAbort();
    void killWorker();

signals:
    void bytesTransfered(qint64);
    void objectsTransfered(uint, uint);
    void error(int code, const QString& message);

private:
    QString m_url;
    QString m_masterFilename;
    QString m_remoteFilename;

    QThread* m_thread;
    Worker* m_worker;
    bool m_abortFlag;
    bool m_error;

public:
    bool commit(git_repository* repo, const QString& filename, const QString &author, const QString &email, const QString& message);

    /**
     * @brief startPush starts the push in another thread.
     *  Call @code isFinished to check if the pushing finished.
     *  @code error returns whether an error occured.
     * @param repository the repository
     * @param[out] remote you must free the remote after the push finishes.
     * @param refspecs
     * @param options
     */
    void startPush(git_repository* repository, git_remote *&remote, git_strarray* refspecs, const git_push_options *options);
    void startClone(git_repository *&repository, const QString& url, const QString& path, const git_clone_options *options);
    bool isAborted() const;
    bool isFinished() const;

public:
    struct Payload
    {
        Payload(GitHandler* git, const QString& username, const QString& password) :
            git(git),
            username(username),
            password(password),
            abortCloneRequested(false)
        {
        }

        GitHandler* git;
        const QString username;
        const QString password;
        bool abortCloneRequested;
    };
};


#endif // GITHANDLER_H
