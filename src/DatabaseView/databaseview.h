#ifndef DATABASEVIEW_H
#define DATABASEVIEW_H

#include <QMenu>
#include <QTableView>

#include "Database/databasesortproxy.h"
#include "global.h"
#include <QKeyEvent>

class OverlayDecorator;
class DatabaseViewBase : public QTableView
{
    Q_OBJECT
protected:
    DatabaseViewBase(QWidget* parent = 0);
    void mousePressEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent *e);
    virtual void setFilter(const QString& filter);
    virtual QString filter() const = 0;
    void focusOutEvent(QFocusEvent *event);
    void keyPressEvent(QKeyEvent *event);
    void leaveEvent(QEvent *event);
    void enterEvent(QEvent *event);

    void setModel(QAbstractItemModel *model);

private:
    OverlayDecorator* m_hud;

signals:
    void clicked();
    void changed(); // anything in the database changed (insert, delete, move or edit)
};

template<typename T>
class DatabaseView : public DatabaseViewBase
{
public:
    explicit DatabaseView(DatabaseSortProxy<T>* proxyModel, QWidget *parent = 0) :
        DatabaseViewBase(parent),
        m_proxy( proxyModel )
    {
        m_proxy->setParent(this);
        setAutoScroll(true);
        setContextMenuPolicy(Qt::ActionsContextMenu);
        setAlternatingRowColors( true );
        setEditTriggers( QAbstractItemView::DoubleClicked
                       | QAbstractItemView::EditKeyPressed );
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
        DatabaseViewBase::setModel( m_proxy );

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

    void setFilter(const QString &filter)
    {
        if (m_proxy)
        {
            m_proxy->setFilter(filter);
        }
        DatabaseViewBase::setFilter(filter);
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

private:
    DatabaseSortProxy<T>* m_proxy = nullptr;
    Database<T>* m_model = nullptr;
    QMetaObject::Connection m_connection_updateSelection;
};

#endif // DATABASEVIEW_H
