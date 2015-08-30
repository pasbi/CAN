#include "songdatabaseremovesongcommand.h"

SongDatabaseRemoveSongCommand::SongDatabaseRemoveSongCommand(SongDatabase *songDatabase, Song *song) :
    SongDatabaseCommand( songDatabase ),
    ItemOwnerCommandDecorator( song ),
    m_index(songDatabase->indexOfSong(song).row())
{
    setText( CommandTranslator::tr("Delete Song") );
}

void SongDatabaseRemoveSongCommand::undo()
{
    releaseOwnershipOfItem();
    model()->insertSong(item(), m_index);
}

void SongDatabaseRemoveSongCommand::redo()
{
    takeOwnershipOfItem();
    model()->removeSong(item());
}

