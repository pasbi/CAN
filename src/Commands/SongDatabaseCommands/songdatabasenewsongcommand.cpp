#include "songdatabasenewsongcommand.h"
#include "global.h"

SongDatabaseNewSongCommand::SongDatabaseNewSongCommand(SongDatabase *songDatabase) :
    SongDatabaseCommand(songDatabase)
{
}

SongDatabaseNewSongCommand::~SongDatabaseNewSongCommand()
{
    if (m_ownsSong)
        delete m_song;
}

void SongDatabaseNewSongCommand::redo()
{
    // m_song ownership is transfered to m_songDatabase
    if (!m_song)
        m_song = new Song();

    database()->appendSong( m_song );
    m_ownsSong = false;
}

void SongDatabaseNewSongCommand::undo()
{
    // transfer ownership to this
    assert(m_song);

    database()->removeSong( m_song );
    m_ownsSong = true;
}
