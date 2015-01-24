#ifndef SONGDATABASEREMOVESONGCOMMAND_H
#define SONGDATABASEREMOVESONGCOMMAND_H

#include "songdatabasecommand.h"

class SongDatabaseRemoveSongCommand : public SongDatabaseCommand
{
public:
    /**
     * @brief SongDatabaseRemoveSongCommand
     * @param songDatabase
     * @param song ownership is transfered to this when redo and transfered back to songDatabase on undo.
     *          song is deleted if this is deleted and owns song.
     */
    SongDatabaseRemoveSongCommand( SongDatabase* songDatabase, Song* song );
    ~SongDatabaseRemoveSongCommand();

    void undo();
    void redo();

private:
    bool m_ownsSong;
    Song* m_song;
    int m_index;
};

#endif // SONGDATABASEREMOVESONGCOMMAND_H
