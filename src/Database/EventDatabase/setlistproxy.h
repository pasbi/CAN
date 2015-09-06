#ifndef SETLISTPROXY_H
#define SETLISTPROXY_H

#include "Database/databasesortproxy.h"
#include "setlistitem.h"
#include "setlist.h"

class SetlistItem;
class SetlistProxy : public DatabaseSortProxy<SetlistItem>
{
    Q_OBJECT
public:
    explicit SetlistProxy(QObject *parent = 0);

    QVariant data(const QModelIndex &index, int role) const;
    bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const;
};

#endif // SETLISTPROXY_H
