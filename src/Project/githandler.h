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

public slots:
    void abort();
    void killWorker();

signals:
    void bytesTransfered(qint64);
    void objectsTransfered(uint, uint);

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
    void startPush(git_repository* repository, git_remote* &remote, git_strarray* refspecs, const git_push_options *options);
    void startClone(git_repository *&repository, const QString& url, const QString& path, const git_clone_options *options);
    bool isAborted() const;
    bool isFinished() const;

public:
    struct Payload
    {
        Payload(GitHandler* git, const QString& username, const QString& password) :
            git(git),
            username(username),
            password(password)
        {
        }

        GitHandler* git;
        const QString username;
        const QString password;
    };
};


#endif // GITHANDLER_H
