#ifndef DATABASEEDITITEMCOMMAND_H
#define DATABASEEDITITEMCOMMAND_H

#include "databasecommand.h"

template<typename T>
class DatabaseEditItemCommand : public DatabaseCommand<T>
{
public:
    DatabaseEditItemCommand(  Database<T>*          database,
                              const QModelIndex &   index,
                              const QVariant &      newData,
                              const int             role    ) :
        DatabaseCommand<T>(database),
        m_index(index),
        m_newData(newData),
        m_oldData( database->data( index, role ) ),
        m_role(role)
    {
        this->setText( CommandTranslator::tr("edit event") );
    }

    void undo()
    {
        this->model()->setData( m_index, m_oldData, m_role );
    }

    void redo()
    {
        this->model()->setData( m_index, m_newData, m_role );
    }

private:
    const QModelIndex   m_index;
    const QVariant      m_newData;
    const QVariant      m_oldData;
    const int           m_role;
};

#endif // DATABASEEDITITEMCOMMAND_H
