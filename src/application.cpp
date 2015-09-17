#include "application.h"
#include "Project/project.h"
#include "global.h"
#include "mainwindow.h"
#include "Database/SongDatabase/song.h"
#include "Database/SongDatabase/songdatabasesortproxy.h"
#include "DatabaseView/SongDatabaseView/songtableview.h"
#include "FileIndex/fileindex.h"


Application::Application(int &argc, char **argv) :
    QApplication( argc, argv)
{
    setApplicationName("CAN");
    setOrganizationDomain("CAN Developers");

    Configurable::restoreAll();
}

Application::~Application()
{
    delete m_fileIndex;
    m_fileIndex = nullptr;
    Configurable::saveAll();
}

Application & app()
{
    return *static_cast<Application*>(QApplication::instance());
}

void Application::pushCommand(Command *command)
{
    m_project->pushCommand( command );
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

Attachment* Application::currentAttachment() const
{
    return mainWindow()->currentAttachment();
}

FileIndex* Application::fileIndex() const
{
    if (!m_fileIndex)
    {
        m_fileIndex = new FileIndex();
    }
    return m_fileIndex;
}

