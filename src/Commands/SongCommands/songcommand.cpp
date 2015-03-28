#include "songcommand.h"

SongCommand::SongCommand(Song *song) :
    m_song ( song )
{
    m_type = SongDatabaseRelated;
}
