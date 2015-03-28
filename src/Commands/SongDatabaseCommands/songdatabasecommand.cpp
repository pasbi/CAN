#include "songdatabasecommand.h"

SongDatabaseCommand::SongDatabaseCommand(SongDatabase *songDatabase) :
    m_songDatabase(songDatabase)
{
    m_type = SongDatabaseRelated;
}
