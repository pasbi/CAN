#include "songtableview.h"

SongTableView::SongTableView(QWidget *parent) :
    QTableView(parent)
{
}

bool SongTableView::edit(const QModelIndex &index, EditTrigger trigger, QEvent *event)
{
    return false;
}
