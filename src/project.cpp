#include "project.h"

Project::Project() : GitRepository("can")
{
    m_songDatabase = new SongDatabase();
    m_dateDatabase = new DateDatabase();
}

Project::~Project()
{
    delete m_songDatabase;
    delete m_dateDatabase;
}

bool Project::loadFromTempDir()
{
    return false;
}

bool Project::saveToTempDir()
{
    return false;
}
