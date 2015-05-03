#ifndef SONGDATABASECOMMAND_H
#define SONGDATABASECOMMAND_H

#include "Database/SongDatabase/songdatabase.h"
#include "../command.h"

class SongDatabaseCommand : public Command
{
public:
    SongDatabaseCommand( SongDatabase* songDatabase );
    SongDatabase* database() const { return m_songDatabase; }

private:
    SongDatabase* m_songDatabase;
};

#endif // SONGDATABASECOMMAND_H
