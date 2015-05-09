#ifndef SONGDATABASEMOVESONGCOMMAND_H
#define SONGDATABASEMOVESONGCOMMAND_H

#include "songdatabasecommand.h"

class SongDatabaseMoveSongCommand : public SongDatabaseCommand
{
public:
    SongDatabaseMoveSongCommand( SongDatabase* database, Song* song, int row );
    void undo();
    void redo();

private:

    const int m_sourceRow;
    const int m_targetRow;

    static int getRow( SongDatabase* database, Song* song );

};

#endif // SONGDATABASEMOVESONGCOMMAND_H
