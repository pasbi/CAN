#ifndef PROJECT_H
#define PROJECT_H

#include "gitrepository.h"
#include <QUndoStack>
#include "configurable.h"


class SongDatabase;
class EventDatabase;
class SongDatabaseSortProxy;
class EventDatabaseSortProxy;
class Command;
class Project : public QUndoStack, public GitRepository
{
    DECL_CONFIG(Project)
    Q_OBJECT
public:
    Project();
    ~Project();


    SongDatabase* songDatabase() const { return m_songDatabase; }
    SongDatabaseSortProxy* songDatabaseProxy() const { return m_songDatabaseProxy; }
    EventDatabase* eventDatabase() const { return m_eventDatabase; }
    EventDatabaseSortProxy* eventDatabaseProxy() const { return m_eventDatabaseProxy; }

public slots:
    void pushCommand(Command* command);
    void reset();
    void undo();
    void redo();

signals:
    void songDatabaseCommandPushed();
    void eventDatabaseCommandPushed();
    void commandPushed();
    void canCloseChanged( bool );
    void undoStackCleared();

public:
    bool loadFromTempDir();
    bool saveToTempDir();
    QList<File> getFiles() const;

private:
    SongDatabase* m_songDatabase;
    EventDatabase* m_eventDatabase;
    SongDatabaseSortProxy* m_songDatabaseProxy;
    EventDatabaseSortProxy* m_eventDatabaseProxy;

public:
    bool canClose() const;
    bool isSynchronized() const;
    void setIsSynchronized();
    void markFileRemoved( const QString& filename );
    void setCanClose( bool b );

private:
    bool m_canClose = true;
    bool m_isSynchronized = false;


};

#endif // PROJECT_H
