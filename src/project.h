#ifndef PROJECT_H
#define PROJECT_H

#include "../../../ZipGit/src/zipgit.h"
#include "Database/SongDatabase/songdatabase.h"
#include "Database/DateDatabase/datedatabase.h"

#include "Commands/command.h"
#include <QUndoStack>

class Project : public QUndoStack, public GitRepository
{
    Q_OBJECT
public:
    Project();
    ~Project();

    bool loadFromTempDir();
    bool saveToTempDir();

    SongDatabase* songDatabase() { return &m_songDatabase; }
    DateDatabase* dateDatabase() { return &m_dateDatabase; }

public slots:
    void pushCommand(Command* command);


private:
    SongDatabase m_songDatabase;
    DateDatabase m_dateDatabase;

};

#endif // PROJECT_H
