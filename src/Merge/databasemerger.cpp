#include "databasemerger.h"
#include "Database/SongDatabase/song.h"
#include "Database/EventDatabase/event.h"
#include "Database/EventDatabase/setlist.h"
#include "Attachments/attachment.h"
#include "mergetreeitem.h"

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
    if (master->title() != slave->title())
    {
        return DatabaseMergerPrivate::Unequal;
    }
    if (master->artist() != slave->artist())
    {
        return DatabaseMergerPrivate::Unequal;
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
    if (master->type() != slave->type())
    {
        return DatabaseMergerPrivate::Unequal;
    }
    if (master->label() != slave->label())
    {
        return DatabaseMergerPrivate::Unequal;
    }
    if (master->beginning() != slave->beginning())
    {
        return DatabaseMergerPrivate::Unequal;
    }
    if (master->setlist()->items().length() != slave->setlist()->items().length())
    {
        return DatabaseMergerPrivate::Unequal;
    }
    for (int i = 0; i < master->setlist()->items().length(); ++i)
    {
        if (master->setlist()->items()[i]->type() != slave->setlist()->items()[i]->type())
        {
            return DatabaseMergerPrivate::Unequal;
        }
        if (master->setlist()->items()[i]->song() != slave->setlist()->items()[i]->song())
        {
            return DatabaseMergerPrivate::Unequal;
        }
        if (master->setlist()->items()[i]->label() != slave->setlist()->items()[i]->label())
        {
            return DatabaseMergerPrivate::Unequal;
        }
    }
    return DatabaseMergerPrivate::Equal;
}

template<class T> QString labelItem(const T*)
{
    Q_UNREACHABLE();
}

template<> QString labelItem(const Song* song)
{
    return song->title();
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
MergeTreeItem*  DatabaseMerger<T>::createItem() const
{
    MergeTreeItem* root = new MergeTreeItem();

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

    QList<T*> deletedItems = masterItems; // they are no longer in slave document
    QList<T*> addedItems = slaveItems;    // they are not yet in master document

    for (const T* addedItem : addedItems)
    {
        QString label = QString("%1: %2").arg(QObject::tr("Added")).arg(labelItem(addedItem));
        root->addChild(new MergeTreeItem( MergeInfo::AddItemType, addedItem, { label } ));
    }

    for (const T* deletedItem : deletedItems)
    {
        QString label = QString("%1: %2").arg(QObject::tr("Deleted")).arg(labelItem(deletedItem));
        root->addChild(new MergeTreeItem( MergeInfo::DeleteItemType, deletedItem, { label } ));

    }

    return root;
}



template MergeTreeItem* DatabaseMerger<Song>::createItem() const;
template MergeTreeItem* DatabaseMerger<Event>::createItem() const;
