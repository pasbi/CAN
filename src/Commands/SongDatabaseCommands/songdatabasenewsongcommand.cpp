#include "songdatabasenewsongcommand.h"
#include "global.h"

SongDatabaseNewSongCommand::SongDatabaseNewSongCommand(SongDatabase *songDatabase, Song* song, int row) :
    SongDatabaseCommand(songDatabase),
    m_song(song),
    m_row( row )
{
    if (m_song == NULL)
    {
        m_song = new Song( songDatabase );
    }
}

SongDatabaseNewSongCommand::~SongDatabaseNewSongCommand()
{
    if (m_ownsSong)
        delete m_song;
}

void SongDatabaseNewSongCommand::redo()
{
    // m_song ownership is transfered to m_songDatabase
//    if (!m_song)
//        m_song = new Song(database());

    if (m_row < 0)
    {
        database()->appendSong( m_song );
    }
    else
    {
        database()->insertSong( m_song, m_row );
    }
    m_ownsSong = false;
}

void SongDatabaseNewSongCommand::undo()
{
    // transfer ownership to this
    assert(m_song);

    database()->removeSong( m_song );
    m_ownsSong = true;
}
