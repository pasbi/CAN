#ifndef APPLICATION_H
#define APPLICATION_H

#include <QApplication>
#include "global.h"
#include "FileIndex/fileindex.h"
#include <QSettings>

class Project;
class Command;
class Application : public QApplication
{
    Q_OBJECT
public:
    explicit Application(int & argc, char** argv);
    ~Application();

    FileIndex & fileIndex() { return m_fileIndex; }
    Project* project() const { return m_project; }

    void setProject(Project* project);
    void pushCommand( Command* command );

private:
    FileIndex m_fileIndex;
    QSettings m_settings;

    static Application* m_singleton;
    friend Application & app();

    Project* m_project = NULL;

};

Application & app();

#endif // APPLICATION_H
