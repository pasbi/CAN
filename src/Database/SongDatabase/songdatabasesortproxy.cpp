#include "songdatabasesortproxy.h"

SongDatabaseSortProxy::SongDatabaseSortProxy(QObject *parent) :
    QSortFilterProxyModel( parent)
{
    // filter all columns
    setFilterKeyColumn(-1);
    setDynamicSortFilter( false );
}

bool SongDatabaseSortProxy::lessThan(const QModelIndex &left, const QModelIndex &right) const
{
    if (sourceModel())
    {
        QVariant s1 = sourceModel()->data( left,  Qt::DisplayRole );
        QVariant s2 = sourceModel()->data( right, Qt::DisplayRole );

        return s1 < s2;

    }

    return true;
}
