#ifndef DATABASESORTPROXY_H
#define DATABASESORTPROXY_H

#include <QSortFilterProxyModel>
#include "global.h"

class DatabaseSortProxyBase : public QSortFilterProxyModel
{
    Q_OBJECT
protected:
    DatabaseSortProxyBase(QObject *parent);
    QString filter() const;

public slots:
    void setFilter(const QString& filter);

private:
    QString m_filter;
};

template<typename T> class Database;
template<typename T>
class DatabaseSortProxy : public DatabaseSortProxyBase
{
public:
    explicit DatabaseSortProxy(QObject *parent = 0) :
        DatabaseSortProxyBase(parent)
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


protected:
    bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
    {
        assert(!source_parent.isValid());
        T* item = sourceModel()->items()[source_row];

        for (const QString& token : item->textAttributes())
        {
            if (token.contains(filter(), Qt::CaseInsensitive))
            {
                return true;
            }
        }
        return false;
    }
};

#endif // DATABASESORTPROXY_H
