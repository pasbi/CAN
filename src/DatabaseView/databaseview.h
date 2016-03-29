#ifndef DATABASEVIEW_H
#define DATABASEVIEW_H

#include <QMenu>
#include <QPainter>

#include "Database/databasesortproxy.h"
#include "databaseviewbase.h"
#include <QKeyEvent>

template<typename T>
class DatabaseView : public DatabaseViewBase
{
    typedef DatabaseViewBase Super;
public:
    explicit DatabaseView(DatabaseSortProxy<T>* proxyModel, QWidget *parent = 0) :
        Super(parent),
        m_proxy( proxyModel )
    {
        m_proxy->setParent(this);
    }

    virtual ~DatabaseView()
    {
    }

    QModelIndex indexUnderCursor() const
    {
        QPoint pos = viewport()->mapFromGlobal( QCursor::pos() );
        return indexAt(pos);
    }

    void setModel(Database<T> *model)
    {
        // disconnect old model
        if (m_model)
        {
            disconnect(m_connection_updateSelection);
        }

        // set new model
        m_model = model;
        m_proxy->setSourceModel(model);
        Super::setModel( m_proxy );

        // connect new model
        if (model)
        {
            m_connection_updateSelection = connect( model, &QAbstractTableModel::rowsInserted, [this](QModelIndex index, int first, int last) {
                clearSelection();
                Q_UNUSED(index);
                Q_ASSERT(!index.isValid());
                for (int i = first; i <= last; ++i)
                {
                    selectRow( i );
                }
            });
        }
    }

    Database<T>* sourceModel() const
    {
        return m_model;
    }

    QString filter() const
    {
        if (m_proxy)
        {
            return m_proxy->filter();
        }
        else
        {
            return QString();
        }
    }

    DatabaseSortProxy<T>* sortProxy() const
    {
        return m_proxy;
    }

    void setFilter(const QString &filter)
    {
        if (m_proxy)
        {
            m_proxy->setFilter(filter);
        }
        Super::setFilter(filter);
    }

    T* itemAtIndex(const QModelIndex& index) const
    {
        if (index.model() == m_proxy)
        {
            return m_proxy->itemAtIndex(index);
        }
        else if (index.model() == m_model)
        {
            return m_model->itemAtIndex(index);
        }
        else
        {
            Q_UNREACHABLE();
            return nullptr;
        }
    }

    T* currentItem() const
    {
        QModelIndexList rows = selectionModel()->selectedRows();
        if (rows.isEmpty() || !m_proxy)
        {
            return nullptr;
        }
        else
        {
            return sourceModel()->itemAtIndex( m_proxy->mapToSource(rows.first()) );
        }
    }

    int numFilteredItems() const
    {
        if (m_model && m_proxy)
        {
            return reinterpret_cast<QAbstractItemModel*>(m_model)->rowCount() - reinterpret_cast<QAbstractItemModel*>(m_proxy)->rowCount();
        }
        else
        {
            return 0;
        }
    }

    DatabaseSortProxy<T>* proxyModel() const
    {
        return m_proxy;
    }


private:
    DatabaseSortProxy<T>* m_proxy = nullptr;
    Database<T>* m_model = nullptr;
    QMetaObject::Connection m_connection_updateSelection;
};

#endif // DATABASEVIEW_H
