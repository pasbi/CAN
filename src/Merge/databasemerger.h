#ifndef DATABASEMERGER_H
#define DATABASEMERGER_H

#include "Database/database.h"
#include "mergeitem.h"

class DatabaseMerger
{
public:
    enum CompareResult { Unequal, Equal, Similar };

    DatabaseMerger(const QList<DatabaseItemBase*> &masterItems, const QList<DatabaseItemBase*> &slaveItems);

    // we need a copy of the lists to work on.
    void init(QList<DatabaseItemBase*> masterItems, QList<DatabaseItemBase*> slaveItems);
    virtual CompareResult compare(const DatabaseItemBase*, const DatabaseItemBase*) const { return Unequal; }   //TODO make pure virtual
    MergeItem *join(MergeItem *itemA, MergeItem *itemB);
    QPair<MergeItem *, MergeItem *> split(MergeItem* mergeItem);
    QList<MergeItem*> mergeItems() const;
    MergeItem *decodeMimeData(const QMimeData* mimeData) const;
    QMimeData* encodeMimeData(const MergeItem *mergeItemBase) const;

private:
    QList<MergeItem*> m_mergeItems;

    static bool sortMasterSlaveItem(MergeItem*& masterItem, MergeItem*& slaveItem);
    MergeItem::Action defaultAction(MergeItem::Type) const;

};



#endif // DATABASEMERGER_H
