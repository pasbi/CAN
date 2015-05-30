#include "eventdatabasesortproxy.h"

EventDatabaseSortProxy::EventDatabaseSortProxy(QObject *parent) :
    QSortFilterProxyModel( parent )
{
    // filter all columns
    setFilterKeyColumn(-1);
    setDynamicSortFilter( false );
    setFilterCaseSensitivity( Qt::CaseInsensitive );
}

bool EventDatabaseSortProxy::lessThan(const QModelIndex &left, const QModelIndex &right) const
{
    if (sourceModel())
    {
        QVariant s1 = sourceModel()->data( left,  Qt::DisplayRole );
        QVariant s2 = sourceModel()->data( right, Qt::DisplayRole );

        return s1 < s2;

    }

    return true;
}
