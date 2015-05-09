#include "songdatabasemovesongcommand.h"

int SongDatabaseMoveSongCommand::getRow( SongDatabase* database, Song* song )
{
    for (int i = 0; i < database->m_songs.length(); i++)
    {
        if (database->m_songs[i] == song)
        {
            return i;
        }
    }
    assert( false );
    return -1;
}

SongDatabaseMoveSongCommand::SongDatabaseMoveSongCommand(SongDatabase *database, Song *song, int row) :
    SongDatabaseCommand( database ),
    m_sourceRow( getRow( database, song ) ),
    m_targetRow( row )
{
    qDebug() << "move " << m_sourceRow << m_targetRow;
}

void SongDatabaseMoveSongCommand::undo()
{
    database()->moveRow( m_targetRow, m_sourceRow );
}

void SongDatabaseMoveSongCommand::redo()
{
    database()->moveRow( m_sourceRow, m_targetRow );
}
