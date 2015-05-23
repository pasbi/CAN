#include "databaseview.h"
#include <QMenu>

DatabaseView::DatabaseView(QWidget *parent) :
    QTableView(parent)
{
    setContextMenuPolicy(Qt::CustomContextMenu);
    setAlternatingRowColors( true );
}

QModelIndex DatabaseView::indexUnderCursor() const
{
    QPoint pos = viewport()->mapFromGlobal( QCursor::pos() );
    return indexAt(pos);
}
