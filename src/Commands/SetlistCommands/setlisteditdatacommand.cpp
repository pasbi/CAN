#include "setlisteditdatacommand.h"

SetlistEditDataCommand::SetlistEditDataCommand(Setlist *setlist, const QModelIndex &index, const QVariant &newData, int role) :
    SetlistCommand( setlist ),
    m_index(index),
    m_newData(newData),
    m_role(role)
{
    m_oldData = setlist->data( index, role );
}

void SetlistEditDataCommand::undo()
{
    setlist()->setData_( m_index, m_oldData, m_role);
}

void SetlistEditDataCommand::redo()
{
    setlist()->setData_( m_index, m_newData, m_role);
}
