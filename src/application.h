#ifndef APPLICATION_H
#define APPLICATION_H

#include <QApplication>
#include <QSettings>
#include <QAction>

#include "global.h"

class MainWindow;
class Project;
class Command;
class Song;
class Attachment;
class FileIndex;
class Application : public QApplication
{
    Q_OBJECT
public:
    explicit Application(int & argc, char** argv);
    ~Application();

    FileIndex* fileIndex() const;
    Project* project() const { return m_project; }

    void setProject(Project* project);
    void setMainWindow(MainWindow *mainWindow);

    void pushCommand( Command* command );
    void beginMacro( const QString& text );
    void endMacro();

    void undo() const;
    void redo() const;

    MainWindow* mainWindow() const { return m_mainWindow; }

    Attachment *currentAttachment() const;

private:
    mutable FileIndex* m_fileIndex = nullptr;
    QSettings m_settings;

    static Application* m_singleton;
    friend Application & app();

    Project* m_project = nullptr;
    MainWindow* m_mainWindow = nullptr;

};

Application & app();

#endif // APPLICATION_H
