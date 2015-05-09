#include "databaseview.h"
#include <QMenu>

DatabaseView::DatabaseView(QWidget *parent) :
    QTableView(parent)
{
    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(showContextMenu(QPoint)));
}

QModelIndex DatabaseView::indexUnderCursor() const
{
    QPoint pos = viewport()->mapFromGlobal( QCursor::pos() );
    return indexAt(pos);
}
