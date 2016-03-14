#include "deletesectioncommand.h"
#include "Attachments/AudioAttachment/sectionsmodel.h"

DeleteSectionCommand::DeleteSectionCommand(SectionsModel *model, int row) :
    ModelCommand<SectionsModel>( model ),
    m_row( row )
{
}

void DeleteSectionCommand::redo()
{
    m_section = model()->sections()[ m_row ];
    model()->removeRows( m_row, 1, QModelIndex() );
}

void DeleteSectionCommand::undo()
{
    model()->insertSection( m_section, m_row );
}
