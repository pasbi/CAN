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
}

void DatabaseEditCommand::undo()
{
    model()->setData( m_index, m_oldData, Qt::EditRole );
}

void DatabaseEditCommand::redo()
{
    qDebug() << ">> " << m_newData;
    model()->setData( m_index, m_newData, Qt::EditRole );
    qDebug() << "<< " << model()->data(m_index).toString();
}

