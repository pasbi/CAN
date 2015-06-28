#include "deletesectioncommand.h"

DeleteSectionCommand::DeleteSectionCommand(SectionsModel *model, const int row) :
    SectionsModelCommand( model ),
    m_row( row )
{
    setText( CommandTranslator::tr("delete section") );
}

void DeleteSectionCommand::redo()
{
    m_section = *model()->section( m_row );
    model()->removeRows( m_row, 1, QModelIndex() );
}

void DeleteSectionCommand::undo()
{
    model()->insertSection( m_section, m_row );
}
