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
    qDebug() << "set" <<  m_oldData;
    model()->setData( m_index, m_oldData, Qt::EditRole );
}

void DatabaseEditCommand::redo()
{
    qDebug() << "set" <<  m_newData;
    model()->setData( m_index, m_newData, Qt::EditRole );
}

//void DatabaseEditCommand::undo()
//{
//    qDebug() << model() << app().mainWindow()->ui->eventDatabaseWidget->m_databaseView->m_proxy;
//    qDebug() << app().project()->eventDatabase() << app().mainWindow()->ui->eventDatabaseWidget->m_databaseView->m_proxy->sourceModel();
//    QModelIndex index = app().project()->eventDatabase()->index(m_index.row(), m_index.column());
//    app().mainWindow()->ui->eventDatabaseWidget->m_databaseView->m_proxy->sourceModel()->setData( index, m_oldData, Qt::EditRole );
//}

//void DatabaseEditCommand::redo()
//{
//    qDebug() << "set" <<  m_newData;
//    QModelIndex index = app().project()->eventDatabase()->index(m_index.row(), m_index.column());
//    app().mainWindow()->ui->eventDatabaseWidget->m_databaseView->m_proxy->setData(->sourceModel()->setData( index, m_newData, Qt::EditRole );
//}
