#include "databaseview.h"
#include <QMenu>

DatabaseView::DatabaseView(QWidget *parent) :
    QTableView(parent)
{
    setContextMenuPolicy(Qt::CustomContextMenu);
    setAlternatingRowColors( true );
    setEditTriggers( QAbstractItemView::DoubleClicked
                   | QAbstractItemView::EditKeyPressed );
}

QModelIndex DatabaseView::indexUnderCursor() const
{
    QPoint pos = viewport()->mapFromGlobal( QCursor::pos() );
    return indexAt(pos);
}
