#include "songdatabasetoggleattributevisibility.h"

SongDatabaseToggleAttributeVisibility::SongDatabaseToggleAttributeVisibility(SongDatabase *database, int column, bool state) :
    SongDatabaseCommand( database ),
    m_index( column ),
    m_newState( state )
{

    setText( CommandTranslator::tr("toggle column visibility") );
}

void SongDatabaseToggleAttributeVisibility::undo()
{
    database()->setColumnVisible( m_index, !m_newState );
}

void SongDatabaseToggleAttributeVisibility::redo()
{
    database()->setColumnVisible( m_index, m_newState );
}
