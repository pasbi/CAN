#include "setlisteditdatacommand.h"
#include "Database/EventDatabase/setlist.h"

SetlistEditDataCommand::SetlistEditDataCommand(Setlist *setlist, const QModelIndex &index, const QVariant &newData, int role) :
    ModelCommand( setlist ),
    m_index(index),
    m_newData(newData),
    m_oldData(setlist->data( index, role )),
    m_role(role)
{
    setText( CommandTranslator::tr("edit setlist") );
}

void SetlistEditDataCommand::undo()
{
    model()->setData_( m_index, m_oldData, m_role);
}

void SetlistEditDataCommand::redo()
{
    model()->setData_( m_index, m_newData, m_role);
}
