#include "editsectioncommand.h"

EditSectionCommand::EditSectionCommand(SectionsModel *model, const QVariant &newValue, const QModelIndex & index, int role) :
    ModelCommand<SectionsModel>( model ),
    m_newValue( newValue ),
    m_oldValue( model->data(index, role) ),
    m_index( index ),
    m_role( role )
{
    setText( CommandTranslator::tr("edit section") );
}

void EditSectionCommand::undo()
{
    model()->setData_( m_index, m_oldValue, m_role );
}

void EditSectionCommand::redo()
{
    model()->setData_( m_index, m_newValue, m_role );
}
