#ifndef DATABASEMOVEROWSCOMMAND_H
#define DATABASEMOVEROWSCOMMAND_H

#include <QList>
#include "Commands/modelcommand.h"
#include "Database/databasemimedata.h"

template<typename T> class Database;
template<typename T>
class DatabaseMoveRowsCommand : public ModelCommand<Database<T>>
{
    typedef typename DatabaseMimeData<T>::IndexedItem IndexedItem;
public:
    DatabaseMoveRowsCommand( Database<T>* database,
                             QList<IndexedItem> sortedSource,
                             int targetRow ) :
        ModelCommand<Database<T>>(database),
        m_sortedSource(sortedSource),
        m_recentStatus(this->model()->items()),
        m_targetRow(targetRow)
    {
        this->setText( CommandTranslator::tr("Move %1").arg(this->itemName()) );
    }

    void undo()
    {
        this->model()->beginResetModel();
        this->model()->m_items = m_recentStatus;
        this->model()->endResetModel();
    }

    void redo()
    {
        this->model()->beginResetModel();
        QList<T*> items;
        int n = m_sortedSource.length();
        int insertPos = m_targetRow;
        for (int i = n-1; i >= 0; --i)
        {
            int index = m_sortedSource[i].index;
            items.prepend( this->model()->m_items.takeAt( index ) );
            if (index < insertPos)
            {
                insertPos--;
            }
        }

        for (int i = 0; i < n; ++i)
        {
            this->model()->m_items.insert(insertPos + i, items[i]);
        }
        this->model()->endResetModel();
    }

private:
    const QList<IndexedItem> m_sortedSource;
    const QList<T*> m_recentStatus;
    const int m_targetRow;
};

#endif // DATABASEMOVEROWSCOMMAND_H
