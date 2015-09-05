#include "databasesortproxy.h"

DatabaseSortProxyBase::DatabaseSortProxyBase(QObject *parent) :
    QSortFilterProxyModel(parent)
{
}

void DatabaseSortProxyBase::setFilter(const QString &filter)
{
    qDebug() << "DatabaseSortProxyBase::setfilter" << filter;
    m_filter = filter;
    invalidate();
}

QString DatabaseSortProxyBase::filter() const
{
    return m_filter;
}
