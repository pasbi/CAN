#include "databasemergerbase.h"

DatabaseMergerBase::DatabaseMergerBase()
{
}

DatabaseMergerBase::~DatabaseMergerBase()
{

}

MergeItem* DatabaseMergerBase::join(MergeItem* itemA, MergeItem* itemB)
{
    Q_ASSERT(m_mergeItems.contains(itemA));
    Q_ASSERT(m_mergeItems.contains(itemB));

    // keep ordering
    m_mergeItems.removeOne(itemB);
    int index = m_mergeItems.indexOf(itemA);
    m_mergeItems.removeOne(itemA);

    MergeItem* masterItem = itemA;
    MergeItem* slaveItem = itemB;
    MergeItem::sortMasterSlaveItem(masterItem, slaveItem);

    MergeItem* joinedItem = new MergeItem(masterItem->master(), slaveItem->slave(), MergeItem::ModifyAction);
    m_mergeItems.insert(index, joinedItem);

    delete itemA;
    delete itemB;
    itemA = nullptr;
    itemB = nullptr;
    masterItem = nullptr;
    slaveItem = nullptr;

    if (DatabaseMergerBase* child = childDatabaseMerger(joinedItem->master(), joinedItem->slave()))
    {
        m_children.insert( joinedItem, child );
    }


    return joinedItem;
}

QPair<MergeItem*, MergeItem*> DatabaseMergerBase::split(MergeItem* mergeItem)
{
    Q_ASSERT(m_mergeItems.contains(mergeItem));
    int index = m_mergeItems.indexOf(mergeItem);
    m_mergeItems.removeOne(mergeItem);
    MergeItem* masterItem = new MergeItem(mergeItem->master(), MergeItem::Master, defaultAction(MergeItem::Master));
    MergeItem* slaveItem = new MergeItem(mergeItem->slave(), MergeItem::Slave, defaultAction(MergeItem::Slave));
    m_mergeItems.insert(index, masterItem);
    m_mergeItems.insert(index + 1, slaveItem);

    delete mergeItem;
    mergeItem = nullptr;

    return qMakePair(masterItem, slaveItem);
}

QList<MergeItem*> DatabaseMergerBase::mergeItems() const
{
    return m_mergeItems;
}

MergeItem *DatabaseMergerBase::decodeMimeData(const QMimeData* mimeData) const
{
    Q_ASSERT(mimeData->hasFormat("merge"));
    QDataStream stream(mimeData->data("merge"));
    quintptr intptr;
    stream >> intptr;
    MergeItem* mergeItem = static_cast<MergeItem*>(reinterpret_cast<void*>(intptr));
    Q_ASSERT( m_mergeItems.contains(mergeItem) );
    return mergeItem;
}

QMimeData* DatabaseMergerBase::encodeMimeData(const MergeItem *mergeItemBase) const
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

MergeItem::Action DatabaseMergerBase::defaultAction(MergeItem::Origin origin) const
{
    switch (origin)
    {
    case MergeItem::Slave:
        return preference<MergeItem::Action>("defaultActionMergeSlave");
    case MergeItem::Master:
        return preference<MergeItem::Action>("defaultActionMergeMaster");
    case MergeItem::Both:
    default:
        Q_UNREACHABLE();
        return MergeItem::AddAction;
    }
}

double DatabaseMergerBase::similarThreshold() const
{
    return preference<double>("MergeSimilarityThreshold");
}

//void DatabaseMergerBase::addChild(DatabaseMergerBase* child)
//{
//    m_children << child;
//}

QMap<MergeItem *, DatabaseMergerBase *> DatabaseMergerBase::children() const
{
//    return m_children;
    return QMap<MergeItem *, DatabaseMergerBase *>();
}


