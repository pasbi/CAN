#ifndef DATABASEVIEW_H
#define DATABASEVIEW_H

#include <QMenu>
#include <QTableView>

#include "Database/databasesortproxy.h"
#include "global.h"

class DatabaseViewBase : public QTableView
{
    Q_OBJECT
protected:
    DatabaseViewBase(QWidget* parent = 0);
    void mousePressEvent(QMouseEvent *event);
signals:
    void clicked();
};

template<typename T>
class DatabaseView : public DatabaseViewBase
{
public:
    explicit DatabaseView(QWidget *parent = 0) :
        DatabaseViewBase(parent)
    {
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

    void setModel(DatabaseSortProxy<T> *model)
    {
        QTableView::setModel( model );
        connect( model->sourceModel(), &QAbstractTableModel::rowsInserted, [this](QModelIndex, int last) {
            selectRow( last );
        });
    }

    DatabaseSortProxy<T>* proxyModel() const
    {
        return static_cast<DatabaseSortProxy<T>*>(DatabaseViewBase::model());
    }

    Database<T>* model() const
    {
        QAbstractItemModel* model = DatabaseViewBase::model();
        while (model && model->inherits("QAbstractProxyModel"))
        {
            model = qobject_assert_cast<QAbstractProxyModel*>(model)->sourceModel();
        }

        return static_cast<Database<T>*>(model);
    }
};

#endif // DATABASEVIEW_H
