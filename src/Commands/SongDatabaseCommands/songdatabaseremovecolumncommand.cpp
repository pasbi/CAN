#include "songdatabaseremovecolumncommand.h"

SongDatabaseRemoveColumnCommand::SongDatabaseRemoveColumnCommand(SongDatabase *songDatabase, const int section) :
    SongDatabaseCommand(songDatabase),
    m_section(section)
{
}

void SongDatabaseRemoveColumnCommand::undo()
{
    database()->insertColumn( m_section, m_label );
    for ( Song* s : database()->songs() )
    {
        s->setAttribute( m_section, m_attributes.takeFirst() );
    }
}

void SongDatabaseRemoveColumnCommand::redo()
{
    m_attributes.clear();
    m_label = database()->attributeKeys()[m_section];
    for (Song* s : database()->songs())
    {
        m_attributes.append(s->attribute( m_section ));
    }

    database()->removeColumns( m_section, 1, QModelIndex() );
}
