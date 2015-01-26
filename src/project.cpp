#include "project.h"
#include "global.h"

Project::Project() :
    GitRepository("can"),
    m_songDatabase( new SongDatabase(this) ),
    m_songDatabaseProxy( new SongDatabaseSortProxy(this) ),
    m_dateDatabase( new DateDatabase(this) )
{
    m_songDatabaseProxy->setSourceModel(m_songDatabase);
}

Project::~Project()
{
    delete m_songDatabase;
    delete m_dateDatabase;
    delete m_songDatabaseProxy;
}

bool Project::loadFromTempDir()
{
    bool success = true;
    if (!m_dateDatabase->loadFrom(makeAbsolute("dateDatabase")))
    {
        WARNING << "Cannot load Date Database";
        success = false;
    }
    if (!m_songDatabase->loadFrom(makeAbsolute("songDatabase")))
    {
        WARNING << "Cannot load Song Database";
        success = false;
    }
    return success;
}

bool Project::saveToTempDir()
{
    bool success = true;
    if (!m_dateDatabase->saveTo(makeAbsolute("dateDatabase")))
    {
        WARNING << "Cannot save Date Database.";
        success = false;
    }
    if (!m_songDatabase->saveTo(makeAbsolute("songDatabase")))
    {
        WARNING << "Cannot save Song Database";
        success = false;
    }

    return success;
}

void Project::pushCommand(Command *command)
{
    QUndoStack::push(command);
}
