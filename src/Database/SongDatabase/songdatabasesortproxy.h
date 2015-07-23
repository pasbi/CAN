#ifndef SONGDATABASESORTPROXY_H
#define SONGDATABASESORTPROXY_H

#include <QSortFilterProxyModel>
#include "songdatabase.h"

class SongDatabaseSortProxy : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    explicit SongDatabaseSortProxy(QObject* parent = 0);

    void setSourceModel(SongDatabase *sourceModel);
    SongDatabase* sourceModel() const;

protected:
    bool filterAcceptsColumn(int source_column, const QModelIndex &source_parent) const;

};

#endif // SONGDATABASESORTPROXY_H
