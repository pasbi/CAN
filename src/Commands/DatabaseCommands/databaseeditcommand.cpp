#include "databaseeditcommand.h"
#include "Database/database.h"

DatabaseEditCommand::DatabaseEditCommand( QAbstractItemModel*   database,
                                          const QModelIndex &   index,
                                          const QVariant &      newData ) :
    ModelCommand(database),
    m_index(index),
    m_newData(newData),
    m_oldData( database->data( index, Qt::EditRole ) )
{
    setText( CommandTranslator::tr("Edit %1").arg(itemTypeName()) );
}

void DatabaseEditCommand::undo()
{
    model()->setData( m_index, m_oldData, Qt::EditRole );
}

void DatabaseEditCommand::redo()
{
    model()->setData( m_index, m_newData, Qt::EditRole );
}

