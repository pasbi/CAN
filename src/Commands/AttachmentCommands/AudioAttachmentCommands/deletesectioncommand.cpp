#include "deletesectioncommand.h"

DeleteSectionCommand::DeleteSectionCommand(SectionsModel *model, const int row) :
    SectionsModelCommand( model ),
    m_row( row )
{
}

void DeleteSectionCommand::redo()
{
    m_section = *model()->section( m_row );
    setText( QString(CommandTranslator::tr("Delete Section [%1 - %2]")).arg(m_section.begin()).arg(m_section.end()) );
    model()->removeRows( m_row, 1, QModelIndex() );
}

void DeleteSectionCommand::undo()
{
    model()->insertSection( m_section, m_row );
}
