#ifndef SONGDATABASESORTPROXY_H
#define SONGDATABASESORTPROXY_H

#include "Database/databasesortproxy.h"
#include "songdatabase.h"

class SongDatabaseSortProxy : public DatabaseSortProxy<Song>
{
    Q_OBJECT
public:
    explicit SongDatabaseSortProxy(QObject* parent = 0);

protected:
    bool filterAcceptsColumn(int source_column, const QModelIndex &source_parent) const;

};

#endif // SONGDATABASESORTPROXY_H
