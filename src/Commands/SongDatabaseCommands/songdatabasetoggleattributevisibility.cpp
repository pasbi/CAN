#include "songdatabasetoggleattributevisibility.h"

SongDatabaseToggleAttributeVisibility::SongDatabaseToggleAttributeVisibility(SongDatabase *database, int column, bool state) :
    SongDatabaseCommand( database ),
    m_index( column ),
    m_newState( state )
{

}

void SongDatabaseToggleAttributeVisibility::undo()
{
    database()->setAttributeVisible( m_index, !m_newState );
}

void SongDatabaseToggleAttributeVisibility::redo()
{
    database()->setAttributeVisible( m_index, m_newState );
}
