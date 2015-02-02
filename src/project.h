#ifndef PROJECT_H
#define PROJECT_H

#include "../../../ZipGit/src/zipgit.h"
#include "Database/SongDatabase/songdatabase.h"
#include "Database/DateDatabase/datedatabase.h"

#include "Commands/command.h"
#include <QUndoStack>
#include "Database/SongDatabase/songdatabasesortproxy.h"

class Project : public QUndoStack, public GitRepository
{
    Q_OBJECT
public:
    Project();
    ~Project();

    bool loadFromTempDir();
    bool saveToTempDir();

    SongDatabase* songDatabase() const { return m_songDatabase; }
    SongDatabaseSortProxy* songDatabaseProxy() const { return m_songDatabaseProxy; }

    DateDatabase* dateDatabase() const { return m_dateDatabase; }

public slots:
    void undo();
    void redo();

signals:
    void undoPerformed();
    void redoPerformed();


public slots:
    void pushCommand(Command* command);


private:
    SongDatabase* m_songDatabase;
    SongDatabaseSortProxy* m_songDatabaseProxy;
    DateDatabase* m_dateDatabase;


};

#endif // PROJECT_H
