#include "insertsectioncommand.h"
#include "global.h"
#include "Attachments/AudioAttachment/sectionsmodel.h"

InsertSectionCommand::InsertSectionCommand(SectionsModel *model, Section section, int index) :
    ModelCommand( model ),
    m_section( section ),
    m_index( index < 0 ? model->rowCount() : index )
{
}

void InsertSectionCommand::undo()
{
    model()->removeSection( m_index );
}

void InsertSectionCommand::redo()
{
    model()->insertSection( m_section, m_index );
}
