#include "project.h"
#include <QThread>

#include "Commands/command.h"
#include "global.h"
#include "Database/SongDatabase/songdatabase.h"
#include "Database/EventDatabase/eventdatabase.h"

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

void Project::setCommandFocalizesAffiliatedView(bool on)
{
    m_commandFocalizesAffiliatedView = on;
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
    emit undoStackCleared();
    setCanClose(true);

    return success;
}

bool Project::saveToTempDir()
{
    setCanClose(true);
    return GitRepository::saveToTempDir();
}

QList<File> Project::getFiles() const
{
    QList<File> files;
    files << m_eventDatabase->getFiles();
    files << m_songDatabase->getFiles();
    return files;
}

void emitCommandPushedSignal(Command::Type type, Project* project, bool commandFocalizesAffiliatedView)
{
    switch (type)
    {
    case Command::SongDatabaseRelated:
        if (commandFocalizesAffiliatedView)
        {
            emit project->songDatabaseCommandPushed();
        }
        break;
    case Command::EventDatabaseRelated:
        if (commandFocalizesAffiliatedView)
        {
            emit project->eventDatabaseCommandPushed();
        }
        break;
    case Command::Other:
        ;
    }
    emit project->commandPushed();
}

void Project::pushCommand(Command *command)
{
    setCanClose(false);

    if (command)
    {
        QUndoStack::push(command);
        emitCommandPushedSignal( command->type(), this, m_commandFocalizesAffiliatedView );
    }

}

void Project::undo()
{
    const QUndoCommand* uc = QUndoStack::command(QUndoStack::index() - 1);
    const Command* c = dynamic_cast<const Command*>( uc );
    QUndoStack::undo();

    if (c)  // cast may fail (e.g. macro command)
    {
        emitCommandPushedSignal( c->type(), this, true );
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
        emitCommandPushedSignal( c->type(), this, true );
    }

    setCanClose( false );

}

void Project::reset()
{
    m_songDatabase->reset();
    m_eventDatabase->reset();
    QUndoStack::clear();
    emit undoStackCleared();
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



