#include "databasemerger.h"
#include "Database/SongDatabase/song.h"
#include "Database/EventDatabase/event.h"
#include "Database/EventDatabase/setlist.h"
#include "Attachments/attachment.h"
#include "mergeitem.h"

template<class T> DatabaseMergerPrivate::CompareResult compare(const T*, const T*)
{
    Q_UNREACHABLE();
    return DatabaseMergerPrivate::Unequal;
}

// make this member functions of song, event, attachment? return score (0 = equal, inf = different)
template<> DatabaseMergerPrivate::CompareResult compare(const Attachment*, const Attachment*)
{
    return DatabaseMergerPrivate::Equal;
}

template<> DatabaseMergerPrivate::CompareResult compare(const Song* master, const Song* slave)
{
    //TODO recognize renamings
    for (const QString& key : Song::ATTRIBUTE_KEYS)
    {
        if (master->attribute(key) != slave->attribute(key))
        {
            return DatabaseMergerPrivate::Unequal;
        }
    }

    if (master->attachments().length() != slave->attachments().length())
    {
        return DatabaseMergerPrivate::Unequal;
    }

    for (int i = 0; i < master->attachments().length(); ++i)
    {
        switch (compare(master->attachment(i), slave->attachment(i)))
        {
        case DatabaseMergerPrivate::Unequal:
            return DatabaseMergerPrivate::Unequal;
        default:
            ;
        }
    }
    return DatabaseMergerPrivate::Equal;
}

template<> DatabaseMergerPrivate::CompareResult compare(const Event* master, const Event* slave)
{
    //TODO recognize renamings
    for (const QString& key : Event::ATTRIBUTE_KEYS)
    {
        if (master->attribute(key) != slave->attribute(key))
        {
            return DatabaseMergerPrivate::Unequal;
        }
    }

    //TODO check setlist

    return DatabaseMergerPrivate::Equal;
}

template<class T> QString labelItem(const T*)
{
    Q_UNREACHABLE();
}

template<> QString labelItem(const Song* song)
{
    return song->attribute("title").toString();
}


template<> QString labelItem(const Event* event)
{
    return event->description();
}


template<> QString labelItem(const Attachment* attachment)
{
    return attachment->name();
}

template<class T>
QList<MergeItemBase*> DatabaseMerger<T>::createItems() const
{
    QList<MergeItemBase*> items;

    // get all items
    QList<T*> masterItems = m_master->items();
    QList<T*> slaveItems = m_slave->items();

    // remove items that are in master-list and in slave list (nothing changed there!)
    QMutableListIterator<T*> mi(masterItems);
    while (mi.hasNext())
    {
        const T* item = mi.next();
        bool buddyFound = false;
        for (int si = 0; si < slaveItems.length() && !buddyFound; ++si)
        {
            if (compare(item, slaveItems[si]) == DatabaseMergerPrivate::Equal)
            {
                mi.remove();
                slaveItems.removeAt(si);
                buddyFound = true;
            }
        }
    }

    //TODO filter those songs, where an attachment was deleted / added

    for (T* masterItem : masterItems)
    {
        items << new MergeItem<T>( MergeItemBase::MasterProject, masterItem, labelItem(masterItem) );
    }

    for (T* slaveItem : slaveItems)
    {
        items << new MergeItem<T>( MergeItemBase::SlaveProject, slaveItem, labelItem(slaveItem) );
    }

    return items;
}



template QList<MergeItemBase*> DatabaseMerger<Song>::createItems() const;
template QList<MergeItemBase*> DatabaseMerger<Event>::createItems() const;
