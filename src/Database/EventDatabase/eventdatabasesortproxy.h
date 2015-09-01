#ifndef EVENTDATABASESORTPROXY_H
#define EVENTDATABASESORTPROXY_H

#include "Database/databasesortproxy.h"

class EventDatabase;
class Event;
class EventDatabaseSortProxy : public DatabaseSortProxy<Event>
{
    Q_OBJECT
public:
    explicit EventDatabaseSortProxy(QObject* parent = 0);

    bool lessThan(const QModelIndex &left, const QModelIndex &right) const;
};

#endif // EVENTDATABASESORTPROXY_H
