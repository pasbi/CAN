#include "project.h"
#include "global.h"
#include <QThread>

DEFN_CONFIG(Project, "Project");

Project::Project() :
    GitRepository("can"),
    m_songDatabase( new SongDatabase(this) ),
    m_eventDatabase( new EventDatabase(this) ),
    m_songDatabaseProxy( new SongDatabaseSortProxy(this) ),
    m_eventDatabaseProxy( new EventDatabaseSortProxy(this) )
{
    reset();
    m_songDatabaseProxy->setSourceModel( m_songDatabase );
    m_eventDatabaseProxy->setSourceModel( m_eventDatabase );
}

Project::~Project()
{
    delete m_songDatabase;
    delete m_eventDatabase;
}

void Project::setCanClose(bool b)
{
    if (m_canClose != b)
    {
        m_canClose = b;
        emit canCloseChanged(b);
    }
}

bool Project::loadFromTempDir()
{
    bool success = true;


    if (!m_songDatabase->loadFrom(makeAbsolute("songDatabase")))
    {
        WARNING << "Cannot load Song Database";
        success = false;
    }

    // event database must be restored after song database.
    if (!m_eventDatabase->loadFrom(makeAbsolute("eventDatabase")))
    {
        WARNING << "Cannot load Date Database";
        success = false;
    }



    QUndoStack::clear();
    setCanClose(true);

    return success;
}

bool Project::saveToTempDir()
{
    bool success = true;

    if ( !resetUserData() )
    {
        WARNING << "Cannot tidy up temporary directory.";
        success = false;
    }

    if (!m_eventDatabase->saveTo(makeAbsolute("eventDatabase")))
    {
        WARNING << "Cannot save Event Database.";
        success = false;
    }
    if (!m_songDatabase->saveTo(makeAbsolute("songDatabase")))
    {
        WARNING << "Cannot save Song Database";
        success = false;
    }
    setCanClose(true);
    return success;
}

void Project::pushCommand(Command *command)
{
    setCanClose(false);
    QUndoStack::push(command);

    emitCommandPushedSignal( command->type() );

}

void Project::emitCommandPushedSignal(Command::Type type)
{
    switch (type)
    {
    case Command::SongDatabaseRelated:
        emit songDatabaseCommandPushed();
        break;
    case Command::EventDatabaseRelated:
        emit eventDatabaseCommandPushed();
        break;
    default:
        ;
    }
}

void Project::undo()
{
    const QUndoCommand* uc = QUndoStack::command(QUndoStack::index() - 1);
    const Command* c = dynamic_cast<const Command*>( uc );

    if (c)  // cast may fail (e.g. command-macro)
    {
        emitCommandPushedSignal( c->type() );
    }

    setCanClose( false );

    QUndoStack::undo();
}

void Project::redo()
{
    const QUndoCommand* uc = QUndoStack::command(QUndoStack::index());
    const Command* c = dynamic_cast<const Command*>( uc );
    if (c)  // cast may fail (e.g. command-macro)
    {
        emitCommandPushedSignal( c->type() );
    }

    setCanClose( false );

    QUndoStack::redo();
}

void Project::reset()
{
    m_songDatabase->reset();
    m_eventDatabase->reset();
    QUndoStack::clear();
}

bool Project::canClose() const
{
    return m_canClose;
}


// Sync in another thread

class SyncThread : public QThread
{
public:
    SyncThread( GitRepository* repo, const QString & message, const Identity & identity ) :
        m_repository( repo ),
        m_message( message ),
        m_identity( identity )
    {

    }

    bool success() const { return m_success; }

    void run()
    {
        m_success = m_repository->sync( m_message, m_identity );
    }

private:
    GitRepository* m_repository;
    QString m_message;
    Identity m_identity;
    bool m_success;
};

class CloneThread : public QThread
{
public:
    CloneThread( GitRepository* repo, const QString &url ) :
        m_repository( repo ),
        m_url( url )
    {

    }

    bool success() const { return m_success; }

    void run()
    {
        m_success = m_repository->clone( m_url );
    }

private:
    GitRepository* m_repository;
    QString m_url;
    bool m_success;
};

void Project::syncDetached( const QString & message, const Identity& identity )
{
    SyncThread* thread = new SyncThread( this, message, identity );

    m_syncFinished = false;
    QObject::connect( thread, &QThread::finished, [this, thread]()
    {
        thread->deleteLater();
        m_syncResult = thread->success();
        m_syncFinished = true;  // set this the last.
    });

    thread->start();
}

void Project::cloneDetached( const QString &url )
{
    CloneThread* thread = new CloneThread( this, url );

    m_cloneFinished = false;
    QObject::connect( thread, &QThread::finished, [this, thread]()
    {
        thread->deleteLater();
        m_cloneResult = thread->success();
        m_cloneFinished = true;  // set this the last.
    });

    thread->start();
}


