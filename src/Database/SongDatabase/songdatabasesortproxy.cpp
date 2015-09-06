#include "songdatabasesortproxy.h"


SongDatabaseSortProxy::SongDatabaseSortProxy(QObject *parent) :
    DatabaseSortProxy(parent)
{
    // filter all columns
    setFilterKeyColumn(-1);
    setDynamicSortFilter( false );
    setFilterCaseSensitivity( Qt::CaseInsensitive );
}
