#include "project.h"
#include "global.h"

DEFN_CONFIG(Project, "Project");

Project::Project() :
    GitRepository("can"),
    m_songDatabase( new SongDatabase(this) ),
    m_songDatabaseProxy( new SongDatabaseSortProxy(this) ),
    m_dateDatabase( new DateDatabase(this) )
{
    m_songDatabaseProxy->setSourceModel(m_songDatabase);
    reset();
}

Project::~Project()
{
    delete m_songDatabase;
    delete m_dateDatabase;
    delete m_songDatabaseProxy;
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
    if (!m_dateDatabase->loadFrom(makeAbsolute("dateDatabase")))
    {
        qWarning() << "Cannot load Date Database";
        success = false;
    }

    if (!m_songDatabase->loadFrom(makeAbsolute("songDatabase")))
    {
        qWarning() << "Cannot load Song Database";
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
        qWarning() << "Cannot tidy up temporary directory.";
        success = false;
    }

    if (!m_dateDatabase->saveTo(makeAbsolute("dateDatabase")))
    {
        qWarning() << "Cannot save Date Database.";
        success = false;
    }
    if (!m_songDatabase->saveTo(makeAbsolute("songDatabase")))
    {
        qWarning() << "Cannot save Song Database";
        success = false;
    }
    setCanClose(true);
    return success;
}

void Project::pushCommand(Command *command)
{
    setCanClose(false);
    QUndoStack::push(command);
}

void Project::reset()
{
    QUndoStack::clear();
    m_songDatabase->reset();
    m_dateDatabase->reset();
}

bool Project::canClose() const
{
    return m_canClose;
}
