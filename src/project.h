#ifndef PROJECT_H
#define PROJECT_H

#include "../../../ZipGit/src/zipgit.h"
#include "Database/SongDatabase/songdatabase.h"
#include "Database/DateDatabase/datedatabase.h"

#include "Commands/command.h"
#include <QUndoStack>
#include "Database/SongDatabase/songdatabasesortproxy.h"
#include "configurable.h"

class Project : public QUndoStack, public GitRepository
{
    DECL_CONFIG(Project);
    Q_OBJECT
public:
    Project();
    ~Project();


    SongDatabase* songDatabase() const { return m_songDatabase; }
    DateDatabase* dateDatabase() const { return m_dateDatabase; }


public slots:
    void pushCommand(Command* command);
    void reset();
    void undo();
    void redo();

signals:
    void songDatabaseCommandPushed();
    void eventDatabaseCommandPushed();

public:
    bool loadFromTempDir();
    bool saveToTempDir();

private:
    SongDatabase* m_songDatabase;
    DateDatabase* m_dateDatabase;

public:
    bool canClose() const;
private:
    void setCanClose( bool b );
    bool m_canClose = true;
    void resetUndoStack();
    void emitCommandPushedSignal(Command::Type type);
signals:
    void canCloseChanged( bool );

};

#endif // PROJECT_H
