#include "application.h"

Application::Application(int &argc, char **argv) :
    QApplication( argc, argv)
{
    setOrganizationDomain(organizationDomain());
    setObjectName(objectName());
    setApplicationName(applicationName());
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

