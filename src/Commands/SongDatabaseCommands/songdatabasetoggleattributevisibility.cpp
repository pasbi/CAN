#include "songdatabasetoggleattributevisibility.h"

SongDatabaseToggleAttributeVisibility::SongDatabaseToggleAttributeVisibility(SongDatabase *database, int column, bool state) :
    SongDatabaseCommand( database ),
    m_index( column ),
    m_newState( state )
{

    setText(QString(CommandTranslator::tr("Make %1 %2"))
            .arg( database->attributeKeys()[column] )
            .arg( state ? CommandTranslator::tr("visible") : CommandTranslator::tr("invisible") ));
}

void SongDatabaseToggleAttributeVisibility::undo()
{
    database()->setAttributeVisible( m_index, !m_newState );
}

void SongDatabaseToggleAttributeVisibility::redo()
{
    database()->setAttributeVisible( m_index, m_newState );
}
