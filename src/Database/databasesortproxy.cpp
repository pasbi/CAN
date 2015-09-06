#include "databasesortproxy.h"
#include "database.h"

DatabaseSortProxyBase::DatabaseSortProxyBase(QObject *parent) :
    QSortFilterProxyModel(parent)
{
}

void DatabaseSortProxyBase::setFilter(const QString &filter)
{
    m_filter = filter;
    invalidate();
}

QString DatabaseSortProxyBase::filter() const
{
    return m_filter;
}
