#include "songdatabasesortproxy.h"

SongDatabaseSortProxy::SongDatabaseSortProxy(QObject *parent) :
    QSortFilterProxyModel( parent)
{
    // filter all columns
    setFilterKeyColumn(-1);
    setDynamicSortFilter( false );
}

void SongDatabaseSortProxy::setSourceModel(SongDatabase *sourceModel)
{
    QSortFilterProxyModel::setSourceModel(sourceModel);
}

bool SongDatabaseSortProxy::filterAcceptsColumn(int source_column, const QModelIndex &source_parent) const
{
    assert(!source_parent.isValid());
    if (sourceModel())
    {
        return sourceModel()->attributeVisible( source_column );
    }
    else
    {
        return false;
    }
}

SongDatabase* SongDatabaseSortProxy::sourceModel() const
{
     return qobject_assert_cast<SongDatabase*>( QSortFilterProxyModel::sourceModel() );
}
