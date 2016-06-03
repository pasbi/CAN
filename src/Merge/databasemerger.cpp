#include "databasemerger.h"
#include "global.h"
#include "Database/SongDatabase/song.h"
#include "Database/EventDatabase/event.h"
#include "Database/EventDatabase/eventdatabase.h"
#include "Attachments/attachment.h"
#include "Database/EventDatabase/setlistitem.h"
#include "Database/SongDatabase/attachmentdatabase.h"
#include "Database/EventDatabase/setlist.h"

template<class T> DatabaseMerger<T>::DatabaseMerger(Database<T>* masterDatabase, Database<T>* slaveDatabase) :
    m_masterDatabase(masterDatabase),
    m_slaveDatabase(slaveDatabase)
{
    init(masterDatabase->items(),
         slaveDatabase->items() );
}

template<class T> void DatabaseMerger<T>::performMergePrivate(NewPointerTable& updatePointers, QList<const void *> &undeletableItems)
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

    for (MergeItem* mergeItem : mergeItems())
    {
        if (DatabaseMergerBase* child = DatabaseMerger::child(mergeItem))
        {
            child->performMerge(updatePointers, undeletableItems);
        }
    }
}

template<class T> void DatabaseMerger<T>::performMerge(NewPointerTable& updatePointers, QList<const void *> &undeletableItems)
{
    performMergePrivate(updatePointers, undeletableItems);
}

template<> void DatabaseMerger<Event>::performMerge(NewPointerTable& updatePointers, QList<const void *> &undeletableItems)
{
    performMergePrivate(updatePointers, undeletableItems);
    for (MergeItem* item : mergeItems())
    {
        if (item->modifyDetails().contains("setlist"))
        {
            Event* masterEvent = static_cast<Event*>(item->master());
            Event* slaveEvent = static_cast<Event*>(item->slave());

            masterEvent->setSetlist(slaveEvent->setlist());
        }
        else
        {
            // keep master setlist
        }
    }
}

#define SIMILAR_DETECTION
// we need a copy of the lists to work on.
template<class T> void DatabaseMerger<T>::init(const QList<T*>& masterItems, const QList<T*>& slaveItems)
{
#ifdef SIMILAR_DETECTION
    // we want to classify the items in m_onlyMaster, m_onlySlave and m_both and to drop all
    // items that are equal both in master and slave list.


    // sort the <master, slave> pairs by their similarity.
    QMultiMap<double, QPair<T*, T*>> ratioMasterSlaveMap = DatabaseItem<T>::sortSimilar(masterItems, slaveItems);

    for (auto it = ratioMasterSlaveMap.begin(); it != ratioMasterSlaveMap.end(); ++it)
    {
        double ratio = it.key();
        T* master = it.value().first;
        T* slave = it.value().second;

        if (qFuzzyCompare(ratio, 1))
        {
            m_mergeItems.append(new MergeItem(master, slave, MergeItem::NoAction));
        }
        else if (ratio >= ::preference<double>("SimilarityThreshold"))
        {
            qDebug() << "ratio = " << ratio << ::preference<double>("SimilarityThreshold");
            MergeItem* mergeItem = new MergeItem(master, slave, MergeItem::ModifyAction);
            m_mergeItems.append(mergeItem);
            insertChildDatabaseMerger(mergeItem);
        }
        else
        {
            if (master)
#else
            for (T* master : masterItems)
#endif
            {
                m_mergeItems << new MergeItem(master, MergeItem::Master, defaultAction(MergeItem::Master));
            }
#ifdef SIMILAR_DETECTION
            if (slave)
#else
            for (T* slave : slaveItems)
#endif
            {
                m_mergeItems << new MergeItem(slave, MergeItem::Slave, defaultAction(MergeItem::Slave));
            }
#ifdef SIMILAR_DETECTION
        }
}
#endif
}

template<class T> template<class S> QList<T> DatabaseMerger<T>::convertList(const QList<S>& list)
{
    QList<T> convertedList;
    for (const S& s: list)
    {
        convertedList.append(s);
    }
    return convertedList;
}

template<class T> Database<T>* DatabaseMerger<T>::masterDatabase() const
{
    return m_masterDatabase;
}

template<class T> Database<T>* DatabaseMerger<T>::slaveDatabase() const
{
    return m_slaveDatabase;
}

template<class T> DatabaseMergerBase* DatabaseMerger<T>::createChildDatabaseMerger(DatabaseItemBase* masterItem, DatabaseItemBase* slaveItem) const
{
    // child database is very specific and can hardly be implemented for general types.
    // However, only Song and Event need to have child mergers (for Attachment and SetlistItems).
    // returning nullptr indicates that there is no sensible childDatabase for that DatabaseMerger.
    Q_UNUSED(masterItem);
    Q_UNUSED(slaveItem);
    return nullptr;
}

template<> DatabaseMergerBase* DatabaseMerger<Song>::createChildDatabaseMerger(DatabaseItemBase* masterItem, DatabaseItemBase* slaveItem) const
{
    return new DatabaseMerger<Attachment>(static_cast<Song*>(masterItem)->attachmentDatabase(),
                                          static_cast<Song*>(slaveItem)->attachmentDatabase() );
}

template class DatabaseMerger<Song>;
template class DatabaseMerger<Event>;
template class DatabaseMerger<SetlistItem>;
template class DatabaseMerger<Attachment>;

