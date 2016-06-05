#ifndef APPLICATION_H
#define APPLICATION_H

#include <QApplication>
#include <QSettings>
#include <QAction>

#include "global.h"
#include "preferences.h"

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

    template<typename T> T preference(const QString& key) const
    {
        return m_preferences.get(key)->value().value<T>();
    }

    void setPreference(const QString& key, const QVariant& value);
    Preferences* preferences() { return &m_preferences; }

    void handleProjectOpenError(OpenError error, const QString &filename);

    static const QString PROJECT_FILE_FILTER;

    // use them rather than preferences<QString>("dateTimeFormat"/"timeFormat") since they have localized default values.
    QString dateTimeFormat() const;
    QString durationFormat() const;


    // unfortunately, on windows we cannot use self-defined metatypes (apparently Q_DECLARE_METATYPE) does
    // not what it should do. QMap<QString, QString> is the only type we really need to store in preference
    // (the others are defacto-read-only-always-default values. It's fine if we use its default value
    void setPreference(const QString &key, const QMap<QString, QString> &value);
    QMap<QString, QString> stringMapPreference(const QString& key) const;

    bool isValid() const;

private:
    mutable FileIndex* m_fileIndex = nullptr;
    QSettings m_settings;

    static Application* m_singleton;
    friend Application & app();

    Project* m_project = nullptr;
    MainWindow* m_mainWindow = nullptr;

    void initPreferences();
    Preferences m_preferences;

    bool m_isValid;

};

Application & app();

#endif // APPLICATION_H
