#ifndef DATABASEMERGERBASE_H
#define DATABASEMERGERBASE_H

#include <QList>
#include <QMimeData>

#include "mergeitem.h"

class DatabaseItemBase;

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

    virtual DatabaseMergerBase* childDatabaseMerger(DatabaseItemBase* masterItem, DatabaseItemBase* slaveItem) const = 0;

    struct NewPointerTableItem
    {
        const void* master;
        const void* slave;
    };
    typedef QList<NewPointerTableItem> NewPointerTable;

protected:
    double similarThreshold() const;
    QList<MergeItem*> m_mergeItems;

    MergeItem::Action defaultAction(MergeItem::Origin origin) const;


public:
    QMap<MergeItem*, DatabaseMergerBase*> children() const;
    void addChild(DatabaseMergerBase* child);
private:
    QMap<MergeItem*, DatabaseMergerBase*>  m_children;
};


#endif // DATABASEMERGERBASE_H
