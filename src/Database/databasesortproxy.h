#ifndef DATABASESORTPROXY_H
#define DATABASESORTPROXY_H

#include <QSortFilterProxyModel>

template<typename T> class Database;
template<typename T>
class DatabaseSortProxy : public QSortFilterProxyModel
{
public:
    explicit DatabaseSortProxy(QObject *parent = 0) :
        QSortFilterProxyModel(parent)
    {
    }

    Database<T>* sourceModel() const
    {
        return static_cast<Database<T>*>(QSortFilterProxyModel::sourceModel());
    }

    void setSourceModel(Database<T> *sourceModel)
    {
        QSortFilterProxyModel::setSourceModel(sourceModel);
    }
};

#endif // DATABASESORTPROXY_H
