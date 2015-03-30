#include "project.h"
#include "global.h"

DEFN_CONFIG(Project, "Project");

Project::Project() :
    GitRepository("can"),
    m_songDatabase( new SongDatabase(this) ),
    m_eventDatabase( new EventDatabase(this) )
{
    reset();
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
    if (!m_eventDatabase->loadFrom(makeAbsolute("eventDatabase")))
    {
        WARNING << "Cannot load Date Database";
        success = false;
    }

    if (!m_songDatabase->loadFrom(makeAbsolute("songDatabase")))
    {
        WARNING << "Cannot load Song Database";
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
    assert( c );
    emitCommandPushedSignal( c->type() );

    QUndoStack::undo();
}

void Project::redo()
{
    const QUndoCommand* uc = QUndoStack::command(QUndoStack::index());
    const Command* c = dynamic_cast<const Command*>( uc );
    assert( c );
    emitCommandPushedSignal( c->type() );

    QUndoStack::redo();
}

void Project::reset()
{
    QUndoStack::clear();
    m_songDatabase->reset();
    m_eventDatabase->reset();
}

bool Project::canClose() const
{
    return m_canClose;
}
