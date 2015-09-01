#include "insertsectioncommand.h"
#include "global.h"
#include "Attachments/AudioAttachment/sectionsmodel.h"

InsertSectionCommand::InsertSectionCommand(SectionsModel *model, const Section &section, int index) :
    ModelCommand<SectionsModel>( model ),
    m_section( section ),
    m_index( index )
{
    setText( CommandTranslator::tr("new section") );
}

void InsertSectionCommand::undo()
{
    assert( model()->m_sectionsToBeInserted.isEmpty() );
    model()->removeRows( m_index, 1, QModelIndex() );
}

void InsertSectionCommand::redo()
{

    assert( model()->m_sectionsToBeInserted.isEmpty() );
    model()->m_sectionsToBeInserted.append( m_section );
    model()->insertRows( m_index, 1, QModelIndex() );
}
