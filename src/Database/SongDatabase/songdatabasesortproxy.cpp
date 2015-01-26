#include "songdatabasesortproxy.h"

SongDatabaseSortProxy::SongDatabaseSortProxy(QObject *parent) :
    QSortFilterProxyModel(parent)
{
    setFilterKeyColumn(-1);
}
