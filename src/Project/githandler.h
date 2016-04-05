#ifndef GITHANDLER_H
#define GITHANDLER_H

#include <QTemporaryDir>

class git_repository;

/**
 * @brief The GitHandler class handles a remote git repository.
 *  download clones the repository to a temporary folder.
 */
class GitHandler
{
    friend class GitTest;
public:
    GitHandler();
    void setURL(const QString& url);
    QString masterFilename() const;
    void setMasterFilename(const QString& filename);
    QString remoteFilename(const QTemporaryDir& dir) const;
    void setRemoteFilename(const QString& filename);

    /**
     * @brief download downloads (aka clones) the repository at url (@see setURL)
     * @return
     */
    bool download();

    /**
     * @brief sync performs pull, merge, commit, push.
     * @return
     */
    bool sync();

private:
    QString m_url;
    QString m_masterFilename;
    QString m_remoteFilename;

//    bool pull(git_repository *repository);
    bool push(git_repository* repository, const QString &username, const QString &password);
    bool commit();
    bool merge();
    bool clone(git_repository *&repository, const QString& url, const QString& path);



public:
    struct CredentialPayload
    {
        CredentialPayload(const QString& username, const QString& password) :
            username(username),
            password(password)
        {
        }

        const QString username;
        const QString password;
    };
};

#endif // GITHANDLER_H
