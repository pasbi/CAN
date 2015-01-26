#ifndef SONGDATABASENEWSONGCOMMAND_H
#define SONGDATABASENEWSONGCOMMAND_H

#include "songdatabasecommand.h"

class SongDatabaseNewSongCommand : public SongDatabaseCommand
{
public:
    SongDatabaseNewSongCommand(SongDatabase* songDatabase, Song *song);
    ~SongDatabaseNewSongCommand();

    void undo();
    void redo();

private:
    Song* m_song = 0;
    bool m_ownsSong = false;


};

#endif // SONGDATABASENEWSONGCOMMAND_H
