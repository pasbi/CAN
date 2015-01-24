#include "project.h"

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
    return false;
}

bool Project::saveToTempDir()
{
    return false;
}

void Project::pushCommand(Command *command)
{
    QUndoStack::push(command);
}
