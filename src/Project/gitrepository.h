#ifndef GITREPOSITORY_H
#define GITREPOSITORY_H

#include "zipped.h"
#include "identity.h"
#include "conflictfile.h"

// do not derive GitRepository from QObject because each class
// can be derived from OQbject once only and we want to keep this opportunity.
class ErrorSender : public QObject
{
    Q_OBJECT
public:
    void sendError( const QString& message ) { emit error( message ); }
signals:
    void error( QString );
};

class GitRepository : public Zipped
{
public:
    GitRepository(const QString ending);
    virtual ~GitRepository();

    bool loadZip(const QString &filename);
    bool saveZip(const QString &filename);

    /**
     * @brief isGitRepository returns whether therer is a repository.
     * @return
     */
    bool isGitRepository() const;

    /**
     * @brief clone clones a repository from  @code path.
     * @param path the origin
     * @return true on success, false otherwise. @see lastError()
     */
    bool clone(const QString& url);

    /**
     * @brief initialize initialize the repository (do the inital commit)
     * @param identity identity used for commiting
     * @return true on success, false otherwise.
     */
    bool initialize( const Identity& identity );

    bool synchronize(const QString &message, const Identity &identity);
    bool cloneAndInitialize(const QString &url, const Identity& identity);

    bool needsInitialCommit() const;

public:
    /**
     * @brief handleConflicts and return whether there was a conflict resolved.
     *  You must resolve all conflicts in this method, since synchronize cannot
     *  proceed with pending conflicts and will crash if there are any.
     *  Default implementation does nothing and returns false.
     * @return whether there were changes made.
     */
    virtual bool handleConflicts() { return false; }
    bool hasConflicts() const;

    /**
     * @brief createConflictingFiles create a new list of conflicted files. Must be deleted once no more needed.
     * @return
     */
    QList<ConflictFile *> createConflictingFiles() const;

public:
    /**
     * @brief pushOriginMaster push all commits to origin/master
     * @return True on success. On Fail, false is returned and error string is saved in @code m_lastError
     */
    bool pushOriginMaster(const Identity &identity);

    /**
     * @brief pullOriginMaster does a fetch and merge. If there are conflicts, @code handleConflicts is called.
     *  If @code handleConflicts cannot resolve those conflicts, pullOriginMaster returns false.
     *  If @code handleConflicts resolved all conflicts, a proper commit is created.
     * @return True on success. On Fail, false is returned and error string is saved in @code m_lastError
     */
    bool pullOriginMaster();


    /**
     * @brief mergeCommits merges the remote with the local commit
     * @return true on success, false otherwise.
     */
    void mergeCommits( const QString& message, const Identity& identity );


    void commit( const QString& message, const Identity& identity );

    /**
     * @brief errorSender listen to errors
     *  connect( repo->errorSender(), SIGNAL(error(QString)), obj, SLOT(doSthWithErrorMsg(QString)) );
     * @return
     */
    ErrorSender* errorSender() const { return &m_errorSender; }


    void beginIndex() const;
    void endIndex() const;
    void addAllFiles() const;

public:
    void setCredentials(const Identity &identity);
    typedef struct RemotePayload
    {
        QString username;
        QString password;
    } RemotePayload;
private:
    RemotePayload m_payload;

private:
    mutable ErrorSender m_errorSender;
    git_repository* m_repository;
    mutable git_index* m_index;

    /**
     * @brief checkGitCall check whether call succeeded. if not, emit an error.
     * @param errorCode
     * @return
     */
    bool checkGitCall( int errorCode );

    void onAddFile(const QString & absoluteFilename) const;
    void onRemoveFile(const QString & absoluteFilename) const;
    git_commit* getCommit( const QString& ref );


    // detached stuff
public:
    void cloneDetached(const QString& url );
    void initializeDetached(const Identity &identity);
    void pullOriginMasterDetached();
    void pushOriginMasterDetached(const Identity& identity );
    bool detachedTaskFinished() const;
    bool detachedTaskSucceeded() const { return m_succeeded; }
private:
    QThread* m_thread = NULL;
    friend class CloneThread;
    friend class PrepareSyncThread;
    friend class PolishSyncThread;
    bool m_succeeded = false;

};


#endif // GITREPOSITORY_H
