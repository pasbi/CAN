#include "fileindexview.h"

FileIndexView::FileIndexView(QWidget *parent) :
    QTreeView(parent)
{
}

QString FileIndexView::currentPath() const
{
    QModelIndexList selection = selectionModel()->selectedIndexes();
    if (selection.isEmpty())
    {
        return "";
    }
    else
    {
        return model()->entry(selection.first())->absolutePath();
    }
}
