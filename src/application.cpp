#include "application.h"
#include "project.h"

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

