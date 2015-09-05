#ifndef DATABASEEDITITEMCOMMAND_H
#define DATABASEEDITITEMCOMMAND_H

#include "Commands/modelcommand.h"

template<typename T> class Database;
template<typename T>
class DatabaseEditItemCommand : public ModelCommand<Database<T>>
{
public:
    DatabaseEditItemCommand(  Database<T>*          database,
                              const QModelIndex &   index,
                              const QVariant &      newData ) :
        ModelCommand<Database<T>>(database),
        m_index(index),
        m_newData(newData),
        m_oldData( database->data( index, Qt::EditRole ) )
    {
        this->setText( CommandTranslator::tr("Edit %1").arg(this->itemName()) );
    }

    void undo()
    {
        this->model()->setData( m_index, m_oldData, Qt::EditRole );
    }

    void redo()
    {
        this->model()->setData( m_index, m_newData, Qt::EditRole );
    }

private:
    const QModelIndex   m_index;
    const QVariant      m_newData;
    const QVariant      m_oldData;
};

#endif // DATABASEEDITITEMCOMMAND_H
