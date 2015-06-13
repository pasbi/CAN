#include "editsectioncommand.h"

EditSectionCommand::EditSectionCommand(SectionsModel *model, const QVariant &newValue, const QModelIndex & index, const int role) :
    SectionsModelCommand( model ),
    m_newValue( newValue ),
    m_oldValue( model->data(index, role) ),
    m_index( index ),
    m_role( role )
{
}

void EditSectionCommand::undo()
{
    model()->setData_( m_index, m_oldValue, m_role );
    setText(QString(CommandTranslator::tr("Edit section %1 -> %2")).arg( m_oldValue.toString() ).arg( m_newValue.toString() ));
}

void EditSectionCommand::redo()
{
    model()->setData_( m_index, m_newValue, m_role );
}
