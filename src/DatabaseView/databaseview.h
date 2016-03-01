#ifndef DATABASEVIEW_H
#define DATABASEVIEW_H

#include <QMenu>
#include <QPainter>

#include "Database/databasesortproxy.h"
#include "global.h"
#include "freezetableview.h"
#include <QKeyEvent>

class OverlayDecorator;
class DatabaseViewBase : public FreezeTableView
{
    typedef FreezeTableView Super;
    Q_OBJECT
protected:
    DatabaseViewBase(QWidget* parent = 0);
    void mousePressEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent *e);
    virtual void setFilter(const QString& filter);
    virtual QString filter() const = 0;
    void focusOutEvent(QFocusEvent *event);
    void keyPressEvent(QKeyEvent *event);
    void enterEvent(QEvent *event);

    void setModel(QAbstractItemModel *model);
    virtual int numFilteredItems() const = 0;
    QString itemName(int n) const;

private:
    OverlayDecorator* m_hud;
    enum FilterEditMode { OverwriteFilter, AppendFilter };
    FilterEditMode m_filterEditMode;

signals:
    void clicked();
    void changed(); // anything in the database changed (insert, delete, move or edit)
};

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
        setAutoScroll(true);
        setContextMenuPolicy(Qt::ActionsContextMenu);
        setAlternatingRowColors( true );
        setEditTriggers( QAbstractItemView::DoubleClicked
                       | QAbstractItemView::EditKeyPressed );
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
                assert(!index.isValid());
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
            assert(false);
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
