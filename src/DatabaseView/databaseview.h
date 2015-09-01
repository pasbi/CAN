#ifndef DATABASEVIEW_H
#define DATABASEVIEW_H

#include <QMenu>
#include <QTableView>

#include "Database/databasesortproxy.h"
#include "global.h"

template<typename T>
class DatabaseView : public QTableView
{
public:
    explicit DatabaseView(QWidget *parent = 0) :
        QTableView(parent)
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

    DatabaseSortProxy<T>* model() const
    {
        return static_cast<DatabaseSortProxy<T>*>(QTableView::model());
    }
};

#endif // DATABASEVIEW_H
