#include "songdatabasesetcolumnvisibilitycommand.h"

SongDatabaseSetColumnVisibilityCommand::SongDatabaseSetColumnVisibilityCommand(SongDatabase *database, int column, bool state) :
    SongDatabaseCommand( database ),
    m_index( column ),
    m_newState( state )
{

    setText( CommandTranslator::tr("Set Column Visibility") );
}

void SongDatabaseSetColumnVisibilityCommand::undo()
{
    model()->setColumnVisible( m_index, !m_newState );
}

void SongDatabaseSetColumnVisibilityCommand::redo()
{
    model()->setColumnVisible( m_index, m_newState );
}
