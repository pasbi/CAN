#include "songdatabasenewsongcommand.h"
#include "global.h"

SongDatabaseNewSongCommand::SongDatabaseNewSongCommand(SongDatabase *songDatabase, Song* song, int row) :
    SongDatabaseCommand(songDatabase),
    ItemOwnerCommandDecorator(song),
    m_row( row )
{
    setText( CommandTranslator::tr("Add Song") );
}

void SongDatabaseNewSongCommand::redo()
{
    model()->insertSong( item(), m_row );
    releaseOwnershipOfItem();
}

void SongDatabaseNewSongCommand::undo()
{

    model()->removeSong( item() );
    takeOwnershipOfItem();
}
