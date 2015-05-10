#include "songdatabasesortproxy.h"

SongDatabaseSortProxy::SongDatabaseSortProxy(QObject *parent) :
    QSortFilterProxyModel( parent)
{
    // filter all columns
    setFilterKeyColumn(-1);
    setDynamicSortFilter( false );
}
