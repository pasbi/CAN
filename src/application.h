#ifndef APPLICATION_H
#define APPLICATION_H

#include <QApplication>
#include "global.h"
#include "FileIndex/fileindex.h"
#include <QSettings>
#include <QAction>

class MainWindow;
class Project;
class Command;
class Song;
class Application : public QApplication
{
    Q_OBJECT
public:
    explicit Application(int & argc, char** argv);
    ~Application();

    FileIndex & fileIndex() { return m_fileIndex; }
    Project* project() const { return m_project; }

    void setProject(Project* project);
    void setMainWindow(MainWindow *mainWindow);

    void pushCommand( Command* command );
    void beginMacro( const QString& text );
    void endMacro();

    void undo() const;
    void redo() const;

    void selectSong(const Song *song );
    MainWindow* mainWindow() { return m_mainWindow; }

private:
    FileIndex m_fileIndex;
    QSettings m_settings;

    static Application* m_singleton;
    friend Application & app();

    Project* m_project = NULL;
    MainWindow* m_mainWindow = NULL;

};

Application & app();

#endif // APPLICATION_H
