#ifndef PROJECT_H
#define PROJECT_H

#include "../../../ZipGit/src/zipgit.h"
#include "Database/SongDatabase/songdatabase.h"
#include "Database/DateDatabase/datedatabase.h"

class Project : public GitRepository
{
public:
    Project();
    ~Project();

    bool loadFromTempDir();
    bool saveToTempDir();

private:
    SongDatabase* m_songDatabase = 0;
    DateDatabase* m_dateDatabase = 0;

};

#endif // PROJECT_H
