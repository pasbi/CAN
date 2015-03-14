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
    SongDatabaseSortProxy* songDatabaseProxy() const { return m_songDatabaseProxy; }

    DateDatabase* dateDatabase() const { return m_dateDatabase; }


public slots:
    void pushCommand(Command* command);
    void reset();

public:
    bool loadFromTempDir();
    bool saveToTempDir();

private:
    SongDatabase* m_songDatabase;
    SongDatabaseSortProxy* m_songDatabaseProxy;
    DateDatabase* m_dateDatabase;

public:
    bool canClose() const;
private:
    void setCanClose( bool b );
    bool m_canClose = true;
    void resetUndoStack();
signals:
    void canCloseChanged( bool );

};

#endif // PROJECT_H
