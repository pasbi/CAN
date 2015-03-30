#ifndef EVENTDATABASESORTPROXY_H
#define EVENTDATABASESORTPROXY_H

#include <QSortFilterProxyModel>
#include "eventdatabase.h"

class EventDatabaseSortProxy : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    explicit EventDatabaseSortProxy(QObject* parent = 0);

    void setSourceModel(EventDatabase *sourceModel) { QSortFilterProxyModel::setSourceModel(sourceModel); }
    EventDatabase* sourceModel() const { return qobject_assert_cast<EventDatabase*>( QSortFilterProxyModel::sourceModel() ); }

    bool lessThan(const QModelIndex &left, const QModelIndex &right) const;
};

#endif // EVENTDATABASESORTPROXY_H
