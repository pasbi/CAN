#include "application.h"
#include "Project/project.h"
#include "global.h"
#include "mainwindow.h"
#include "Database/SongDatabase/song.h"

Application::Application(int &argc, char **argv) :
    QApplication( argc, argv)
{
    setApplicationName("CAN");
    setOrganizationDomain("CAN Developers");

    Configurable::restoreAll();
}

Application::~Application()
{
    m_fileIndex.save();
    Configurable::saveAll();
}

Application & app()
{
    return *static_cast<Application*>(QApplication::instance());
}

void Application::pushCommand(Command *command)
{
    if (m_project)
    {
        m_project->pushCommand( command );
    }
    else
    {
        qWarning() << "No project set. Cannot push command.";
    }
}

void Application::setProject(Project *project)
{
    m_project = project;
}

void Application::setMainWindow( MainWindow* mainWindow )
{
    m_mainWindow = mainWindow;
}

void Application::undo() const
{
    return project()->undo();
}

void Application::redo() const
{
    return project()->redo();
}

void Application::beginMacro(const QString &text)
{
    m_project->beginMacro( text );
}

void Application::endMacro()
{
    m_project->endMacro();
}

void Application::selectSong(const Song *song)
{
    m_mainWindow->gotoSongView();
    QModelIndex index = m_mainWindow->songTableView()->model()->indexOfSong( song );
    index = m_mainWindow->songTableView()->proxyModel()->mapFromSource( index );
    m_mainWindow->songTableView()->selectRow( index.row() );
}

