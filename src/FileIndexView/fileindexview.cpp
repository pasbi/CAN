#include "fileindexview.h"
#include <QHeaderView>
#include "global.h"
#include <QSortFilterProxyModel>


FileIndexView::FileIndexView(QWidget *parent) :
    QTableView(parent)
{
}

FileIndexView::~FileIndexView()
{
}

void FileIndexView::save() const
{
    model()->save();
}

void FileIndexView::restore()
{
    model()->restore();
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
        return model()->entry(selection.first());
    }
}

FileIndex* FileIndexView::model() const
{
    QSortFilterProxyModel* proxy = dynamic_cast<QSortFilterProxyModel*>( QTableView::model() );
    assert( proxy );
    FileIndex* fi = dynamic_cast<FileIndex*>( proxy->sourceModel() );
    assert( fi );
    return fi;
}

