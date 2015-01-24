#ifndef SONGDATABASESETDATACOMMAND_H
#define SONGDATABASESETDATACOMMAND_H

#include "songdatabasecommand.h"

class SongDatabaseSetDataCommand : public SongDatabaseCommand
{
public:
    SongDatabaseSetDataCommand(SongDatabase* songDatabase);
};

#endif // SONGDATABASESETDATACOMMAND_H
