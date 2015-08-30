#ifndef SONGDATABASENEWSONGCOMMAND_H
#define SONGDATABASENEWSONGCOMMAND_H

#include "Commands/modelcommand.h"
#include "Commands/itemownercommanddecorator.h"
#include "Database/SongDatabase/songdatabase.h"

class SongDatabaseNewSongCommand : public SongDatabaseCommand, private ItemOwnerCommandDecorator<Song>
{
public:
    SongDatabaseNewSongCommand(SongDatabase* songDatabase, Song *song, int row = -1);

    void undo();
    void redo();

private:
    const int m_row;


};

#endif // SONGDATABASENEWSONGCOMMAND_H
