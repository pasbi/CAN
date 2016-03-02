#include "databasemerger.h"
#include "global.h"

DatabaseMerger::DatabaseMerger(const QList<DatabaseItemBase*> &masterItems, const QList<DatabaseItemBase*> &slaveItems)
{
    init(masterItems, slaveItems);
}

DatabaseMerger::~DatabaseMerger()
{

}

// we need a copy of the lists to work on.
void DatabaseMerger::init(QList<DatabaseItemBase*> masterItems, QList<DatabaseItemBase*> slaveItems)
{
    // we want to classify the items in m_onlyMaster, m_onlySlave and m_both and to drop all
    // items that are equal both in master and slave list.

    QMutableListIterator<DatabaseItemBase*> masterIterator(masterItems);
    while (masterIterator.hasNext())
    {
        DatabaseItemBase* masterItem = masterIterator.next();
        bool found = false;
        QMutableListIterator<DatabaseItemBase*> slaveIterator(slaveItems);
        while(slaveIterator.hasNext() && !found)
        {
            DatabaseItemBase* slaveItem = slaveIterator.next();
            switch (compare(masterItem, slaveItem))
            {
            case Similar:
                // move them to a special list.
                found = true;
                m_mergeItems.append(new MergeItem(masterIterator.value(), slaveIterator.value()));
                masterIterator.remove();
                slaveIterator.remove();
                break;
            case Equal:
                // we don't consider equal items here, remove them.
                found = true;
                masterIterator.remove();
                slaveIterator.remove();
                break;
            case Unequal:
                // do nothing, i.e. keep them in list.
                break;
            }
        }
    }

    for (DatabaseItemBase* item : slaveItems)
    {
        m_mergeItems << new MergeItem(item, MergeItem::Add, defaultAction(MergeItem::Add));
    }

    for (DatabaseItemBase* item : masterItems)
    {
        m_mergeItems << new MergeItem(item, MergeItem::Remove, defaultAction(MergeItem::Add));
    }
}


MergeItem* DatabaseMerger::join(MergeItem* itemA, MergeItem* itemB)
{
    Q_ASSERT(m_mergeItems.contains(itemA));
    Q_ASSERT(m_mergeItems.contains(itemB));

    // keep ordering
    m_mergeItems.removeOne(itemB);
    int index = m_mergeItems.indexOf(itemA);
    m_mergeItems.removeOne(itemA);

    MergeItem* masterItem = itemA;
    MergeItem* slaveItem = itemB;
    sortMasterSlaveItem(masterItem, slaveItem);

    MergeItem* joinedItem = new MergeItem(masterItem->master(), slaveItem->slave());
    m_mergeItems.insert(index, joinedItem);

    delete itemA;
    delete itemB;
    itemA = nullptr;
    itemB = nullptr;
    masterItem = nullptr;
    slaveItem = nullptr;

    return joinedItem;
}

QPair<MergeItem*, MergeItem*> DatabaseMerger::split(MergeItem* mergeItem)
{
    Q_ASSERT(m_mergeItems.contains(mergeItem));
    int index = m_mergeItems.indexOf(mergeItem);
    m_mergeItems.removeOne(mergeItem);
    MergeItem* masterItem = new MergeItem(mergeItem->master(), MergeItem::Remove, defaultAction(MergeItem::Remove));
    MergeItem* slaveItem = new MergeItem(mergeItem->slave(), MergeItem::Add, defaultAction(MergeItem::Remove));
    m_mergeItems.insert(index, masterItem);
    m_mergeItems.insert(index + 1, slaveItem);

    delete mergeItem;
    mergeItem = nullptr;

    return qMakePair(masterItem, slaveItem);
}

QList<MergeItem*> DatabaseMerger::mergeItems() const
{
    return m_mergeItems;
}

MergeItem *DatabaseMerger::decodeMimeData(const QMimeData* mimeData) const
{
    Q_ASSERT(mimeData->hasFormat("merge"));
    QDataStream stream(mimeData->data("merge"));
    quintptr intptr;
    stream >> intptr;
    MergeItem* mergeItem = static_cast<MergeItem*>(reinterpret_cast<void*>(intptr));
    Q_ASSERT( m_mergeItems.contains(mergeItem) );
    return mergeItem;
}

QMimeData* DatabaseMerger::encodeMimeData(const MergeItem *mergeItemBase) const
{
    Q_ASSERT(m_mergeItems.contains(const_cast<MergeItem*>(mergeItemBase)));
    quintptr intptr;
    intptr = reinterpret_cast<quintptr>(mergeItemBase);
    QByteArray data;
    QDataStream stream(&data, QIODevice::WriteOnly);
    stream << intptr;
    QMimeData* mimeData = new QMimeData();
    mimeData->setData("merge", data);
    return mimeData;
}


bool DatabaseMerger::sortMasterSlaveItem(MergeItem*& masterItem, MergeItem*& slaveItem)
{
    if (masterItem->type() == MergeItem::Remove && slaveItem->type() == MergeItem::Add)
    {
        return true; // correct order
    }
    if (masterItem->type() == MergeItem::Add && slaveItem->type() == MergeItem::Remove)
    {
        qSwap(masterItem, slaveItem);
        return true; // correct order after swap
    }
    return false;   //two add or two remove types cannot be corrected.
}

MergeItem::Action DatabaseMerger::defaultAction(MergeItem::Type type) const
{
    switch (type)
    {
    case MergeItem::Add:
        return preference<MergeItem::Action>("defaultActionMergeAdd");
    case MergeItem::Remove:
        return preference<MergeItem::Action>("defaultActionMergeRemove");
    case MergeItem::Modify:
    default:
        Q_UNREACHABLE();
        return MergeItem::AddAction;
    }
}

