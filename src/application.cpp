#include "application.h"

Application::Application(int &argc, char **argv) :
    QApplication( argc, argv)
{

}

Application::~Application()
{
    m_fileIndex.save();
}

Application & app()
{
    return *static_cast<Application*>(QApplication::instance());
}
