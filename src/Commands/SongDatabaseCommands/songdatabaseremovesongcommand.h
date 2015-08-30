#ifndef SONGDATABASEREMOVESONGCOMMAND_H
#define SONGDATABASEREMOVESONGCOMMAND_H

#include "Commands/modelcommand.h"
#include "Commands/itemownercommanddecorator.h"
#include "Database/SongDatabase/songdatabase.h"

class SongDatabaseRemoveSongCommand : public SongDatabaseCommand, private ItemOwnerCommandDecorator<Song>
{
public:
    SongDatabaseRemoveSongCommand( SongDatabase* songDatabase, Song* song );

    void undo();
    void redo();

private:
    const int m_index;
};

#endif // SONGDATABASEREMOVESONGCOMMAND_H
