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

    if (!b)
    {
        m_isSynchronized = false;
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

    if (command)
    {
        QUndoStack::push(command);
        emitCommandPushedSignal( command->type() );
    }

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
    case Command::Other:
        ;
    }
    emit commandPushed();
}

void Project::undo()
{
    const QUndoCommand* uc = QUndoStack::command(QUndoStack::index() - 1);
    const Command* c = dynamic_cast<const Command*>( uc );
    QUndoStack::undo();

    if (c)  // cast may fail (e.g. command-macro)
    {
        emitCommandPushedSignal( c->type() );
    }

    setCanClose( false );

}

void Project::redo()
{
    const QUndoCommand* uc = QUndoStack::command(QUndoStack::index());
    const Command* c = dynamic_cast<const Command*>( uc );
    QUndoStack::redo();

    if (c)  // cast may fail (e.g. command-macro)
    {
        emitCommandPushedSignal( c->type() );
    }

    setCanClose( false );

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

bool Project::isSynchronized() const
{
    return m_isSynchronized;
}

void Project::setIsSynchronized()
{
    m_isSynchronized = true;
}



