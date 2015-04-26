#include "editsectioncommand.h"

EditSectionCommand::EditSectionCommand(SectionsModel *sectionsModel, const QVariant &newValue, const QModelIndex & index, const int role) :
    m_newValue( newValue ),
    m_oldValue( sectionsModel->data(index, role) ),
    m_index( index ),
    m_role( role ),
    m_sectionsModel( sectionsModel )
{
}

void EditSectionCommand::undo()
{
    m_sectionsModel->setData_( m_index, m_oldValue, m_role );
}

void EditSectionCommand::redo()
{
    m_sectionsModel->setData_( m_index, m_newValue, m_role );
}
