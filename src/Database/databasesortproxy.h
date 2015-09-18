#ifndef DATABASESORTPROXY_H
#define DATABASESORTPROXY_H

#include <QSortFilterProxyModel>
#include "global.h"
#include "itemhastypenameinterface.h"

class DatabaseBase;
class DatabaseSortProxyBase : public QSortFilterProxyModel
{
    Q_OBJECT
protected:
    DatabaseSortProxyBase(QObject *parent);


public slots:
    virtual void setFilter(const QString& filter);

public:
    QString filter() const;

private:
    QString m_filter;
};

template<typename T> class Database;
template<typename T>
class DatabaseSortProxy : public DatabaseSortProxyBase, public ItemHasTypenameInterface
{
public:
    explicit DatabaseSortProxy(QObject *parent = 0) :
        DatabaseSortProxyBase(parent)
    {
    }

    Database<T>* sourceModel() const
    {
        return static_cast<Database<T>*>(DatabaseSortProxyBase::sourceModel());
    }

    void setSourceModel(Database<T> *sourceModel)
    {
        DatabaseSortProxyBase::setSourceModel(sourceModel);
    }

    T* itemAtIndex(const QModelIndex& index) const
    {
        assert(index.model() == this);
        if (sourceModel())
        {
            return sourceModel()->itemAtIndex( mapToSource(index) );
        }
        else
        {
            return nullptr;
        }
    }

    // for some reason, proxyIndex.parent() is corrupted.
    // fortunately, we don't need it since it is exepected to be invalid.
    // this is a workaround, which works^^
    QModelIndex mapToSource(const QModelIndex &proxyIndex) const
    {
        if (sourceModel())
        {
            return DatabaseSortProxyBase::mapToSource(index(proxyIndex.row(), proxyIndex.column(), QModelIndex()));
        }
        else
        {
            return QModelIndex();
        }
    }

    bool setData(const QModelIndex &index, const QVariant &value, int role)
    {
        if (sourceModel())
        {
            sourceModel()->setData(mapToSource(index), value, role);
            return true;
        }
        else
        {
            return false;
        }
    }

    QString itemTypeName() const
    {
        return sourceModel()->itemTypeName();
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
