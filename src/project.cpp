#include "project.h"
#include "global.h"

Project::Project() :
    GitRepository("can"),
    m_songDatabase(this),
    m_dateDatabase(this)
{
}

Project::~Project()
{
}

bool Project::loadFromTempDir()
{
    bool success = true;
    if (!m_dateDatabase.loadFrom(makeAbsolute("dateDatabase")))
    {
        WARNING << "Cannot load Date Database";
        success = false;
    }
    if (!m_songDatabase.loadFrom(makeAbsolute("songDatabase")))
    {
        WARNING << "Cannot load Song Database";
        success = false;
    }
    return success;
}

bool Project::saveToTempDir()
{
    bool success = true;
    if (!m_dateDatabase.saveTo(makeAbsolute("dateDatabase")))
    {
        WARNING << "Cannot save Date Database.";
        success = false;
    }
    if (!m_songDatabase.saveTo(makeAbsolute("songDatabase")))
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
