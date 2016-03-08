#ifndef DATABASEMERGER_H
#define DATABASEMERGER_H

#include "Database/database.h"
#include "mergeitem.h"

class DatabaseMergerBase
{
public:
    enum CompareResult { Unequal, Equal, Similar };
protected:
    DatabaseMergerBase();
public:
    virtual ~DatabaseMergerBase();

    // we need a copy of the lists to work on.
    void init(QList<DatabaseItemBase*> masterItems, QList<DatabaseItemBase*> slaveItems);


    MergeItem *join(MergeItem *itemA, MergeItem *itemB);
    QPair<MergeItem *, MergeItem *> split(MergeItem* mergeItem);
    QList<MergeItem*> mergeItems() const;
    MergeItem *decodeMimeData(const QMimeData* mimeData) const;
    QMimeData* encodeMimeData(const MergeItem *mergeItemBase) const;

    struct NewPointerTableItem
    {
        const void* master;
        const void* slave;
    };
    typedef QList<NewPointerTableItem> NewPointerTable;

protected:
    double similarThreshold() const;
    QList<MergeItem*> m_mergeItems;

    static bool sortMasterSlaveItem(MergeItem*& masterItem, MergeItem*& slaveItem);
    MergeItem::Action defaultAction(MergeItem::Origin origin) const;
};

template<class T, class S>
QList<T> convertList(const QList<S>& list)
{
    QList<T> convertedList;
    for (const S& s: list)
    {
        convertedList.append(s);
    }
    return convertedList;
}

template<class T>
class DatabaseMerger : public DatabaseMergerBase
{
public:
    DatabaseMerger(Database<T>* masterDatabase, Database<T>* slaveDatabase) :
        m_masterDatabase(masterDatabase),
        m_slaveDatabase(slaveDatabase)
    {
        init(masterDatabase->items(),
             slaveDatabase->items() );
    }

    void performMerge(NewPointerTable& updatePointers, QList<T*>& undeletableItems)
    {
        // do not reuse this DatabaseMerge class.
        Q_ASSERT(undeletableItems.isEmpty());

        for (const MergeItem* mergeItem : mergeItems())
        {
            MergeItem::Action action = mergeItem->action();
            MergeItem::Origin origin = mergeItem->origin();

            if (origin == MergeItem::Master)
            {
                if (action == MergeItem::AddAction)
                {
                    // nothing to do, item is already part of the master document!
                }
                else if (action == MergeItem::RemoveAction)
                {
                    // removing master items might lead to problems. Workaround: only remove non problematic ones
                    T* masterItem = static_cast<T*>(mergeItem->master());
                    if (masterItem->canRemove())
                    {
                        m_masterDatabase->removeItem(masterItem);
                        delete masterItem;
                        masterItem = nullptr;
                    }
                    else
                    {
                        undeletableItems << masterItem;
                    }
                }
                else
                {
                    Q_UNREACHABLE();
                }
            }
            else if (origin == MergeItem::Slave)
            {
                // we have to add the item, regardless what the user says. maybe we can remove it afterwards.
                T* newItem = static_cast<T*>(mergeItem->slave());
                m_slaveDatabase->removeItem(newItem); // remove item from slave database to transfer ownership
                newItem->setDatabase(m_masterDatabase);
                m_masterDatabase->insertItem(newItem);

                if (action == MergeItem::RemoveAction)
                {
                    if (newItem->canRemove())
                    {
                        m_masterDatabase->removeItem(newItem);
                        delete newItem;
                        newItem = nullptr;
                    }
                    else
                    {
                        undeletableItems << newItem;
                    }
                }
                else if (action == MergeItem::AddAction)
                {
                    // is already added, see above.
                }
                else
                {
                    Q_UNREACHABLE();
                }
            }
            else if (origin == MergeItem::Both)
            {
                DatabaseMergerBase::NewPointerTableItem item;
                item.master = mergeItem->master();
                item.slave  = mergeItem->slave();
                if (action == MergeItem::ModifyAction)
                {
                    // do the merge.
                    mergeItem->performModification();
                    Q_ASSERT(m_slaveDatabase->items().contains(static_cast<T*>(mergeItem->slave())));
                    Q_ASSERT(m_masterDatabase->items().contains(static_cast<T*>(mergeItem->master())));
                    updatePointers << item;
                }
                else if (action == MergeItem::NoAction)
                {
                    Q_ASSERT(mergeItem->slave() && mergeItem->master());
                    Q_ASSERT(m_slaveDatabase->items().contains(static_cast<T*>(mergeItem->slave())));
                    Q_ASSERT(m_masterDatabase->items().contains(static_cast<T*>(mergeItem->master())));
                    updatePointers << item;
                }
                else
                {
                    Q_UNREACHABLE();
                }
            }
        }
    }

    // we need a copy of the lists to work on.
    void init(QList<T*> masterItems, QList<T*> slaveItems)
    {
        // we want to classify the items in m_onlyMaster, m_onlySlave and m_both and to drop all
        // items that are equal both in master and slave list.

        QMutableListIterator<T*> masterIterator(masterItems);
        while (masterIterator.hasNext())
        {
            T* masterItem = masterIterator.next();
            bool found = false;
            QMutableListIterator<T*> slaveIterator(slaveItems);
            while(slaveIterator.hasNext() && !found)
            {
                T* slaveItem = slaveIterator.next();
                switch (compare(masterItem, slaveItem))
                {
                case Similar:
                    // move them to a special list.
                    found = true;
                    m_mergeItems.append(new MergeItem(masterIterator.value(), slaveIterator.value(), MergeItem::ModifyAction));
                    masterIterator.remove();
                    slaveIterator.remove();
                    break;
                case Equal:
                    // we don't consider equal items here, remove them.
                    found = true;
                    m_mergeItems.append(new MergeItem(masterIterator.value(), slaveIterator.value(), MergeItem::NoAction));
                    masterIterator.remove();
                    slaveIterator.remove();
                    break;
                case Unequal:
                    // do nothing, i.e. keep them in list.
                    break;
                }
            }
        }

        for (T* item : slaveItems)
        {
            m_mergeItems << new MergeItem(item, MergeItem::Slave, defaultAction(MergeItem::Slave));
        }

        for (T* item : masterItems)
        {
            m_mergeItems << new MergeItem(item, MergeItem::Master, defaultAction(MergeItem::Master));
        }
    }

protected:
    // 1: Equal
    // greater similarThreshold but smaller 0: Similar
    // smaller similarThreshold: Unequal
    // shall be >= 0, <= 1.
    DatabaseMergerBase::CompareResult compare(const T *a, const T *b) const
    {
       if (a == b)
       {
           return DatabaseMergerBase::Equal;
       }
       else
       {
           return DatabaseMergerBase::Unequal;
       }
    }

    double similarity(const T*, const T*) const;

private:
    Database<T>* m_masterDatabase;
    Database<T>* m_slaveDatabase;

};



#endif // DATABASEMERGER_H
