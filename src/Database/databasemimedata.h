#ifndef DATABASEMIMEDATA_H
#define DATABASEMIMEDATA_H

#include <QMimeData>
#include <typeinfo>
#include "global.h"

template<typename T>
class DatabaseMimeData : public QMimeData
{
public:
    typedef struct IndexedItem
    {
        IndexedItem(T* item, int index) : item(item), index(index) {}
        T* item;
        int index;
    } IndexedItem;

    DatabaseMimeData()
    {
    }

    bool hasFormat(const QString &mimetype) const
    {
        return formats().contains(mimetype);
    }

    QStringList formats() const
    {
        return QStringList() << mimeType();
    }

    QList<IndexedItem> indexedItems() const
    {
        return m_indexedItems;
    }

    QList<IndexedItem> indexedItemsSorted() const
    {
        if (m_sortIsDirty)
        {
            m_sortedIndexedItems = m_indexedItems;
            qSort(m_sortedIndexedItems.begin(), m_sortedIndexedItems.end(), [](const IndexedItem& a, const IndexedItem& b){ return a.index < b.index; });
            m_sortIsDirty = false;
        }
        return m_sortedIndexedItems;
    }

    void append(T* item, int index)
    {
        m_sortIsDirty = true;
        m_indexedItems << IndexedItem(item, index);
    }

    static const DatabaseMimeData* cast(const QMimeData* data)
    {
        if (data->hasFormat(mimeType()))
        {
            return static_cast<const DatabaseMimeData<T>*>(data);
        }
        else
        {
            return nullptr;
        }
    }
    static QString mimeType()
    {
        return "Database/" + QString(typeid(T).name());
    }

private:
    QList<IndexedItem> m_indexedItems;

    mutable bool m_sortIsDirty = true;
    mutable QList<IndexedItem> m_sortedIndexedItems;
};

#endif // DATABASEMIMEDATA_H
