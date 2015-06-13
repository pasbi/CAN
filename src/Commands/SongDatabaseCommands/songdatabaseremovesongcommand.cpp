#include "songdatabaseremovesongcommand.h"

SongDatabaseRemoveSongCommand::SongDatabaseRemoveSongCommand(SongDatabase *songDatabase, Song *song) :
    SongDatabaseCommand( songDatabase ),
    m_ownsSong(false),
    m_song(song)
{
    assert( song );
    setText(QString(CommandTranslator::tr("Delete song %1")).arg( song->description() ));
}

SongDatabaseRemoveSongCommand::~SongDatabaseRemoveSongCommand()
{
    if (m_ownsSong)
    {
        delete m_song;
        m_song = 0;
    }
}

void SongDatabaseRemoveSongCommand::undo()
{
    m_ownsSong = false;
    database()->insertSong(m_song, m_index);
}

void SongDatabaseRemoveSongCommand::redo()
{
    m_ownsSong = true;
    m_index = database()->removeSong(m_song);
}

