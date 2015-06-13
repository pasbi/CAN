#include "songdatabasemovesongcommand.h"

SongDatabaseMoveSongCommand::SongDatabaseMoveSongCommand(SongDatabase *database, int sourceRow, int destRow) :
    SongDatabaseCommand( database ),
    m_sourceRow( sourceRow ),
    m_targetRow( destRow )
{
    setText(QString(CommandTranslator::tr("Move song %1 -> %2")).arg( sourceRow, destRow ));
}

void SongDatabaseMoveSongCommand::undo()
{
    database()->moveRow( m_targetRow, m_sourceRow );
}

void SongDatabaseMoveSongCommand::redo()
{
    database()->moveRow( m_sourceRow, m_targetRow );
}
