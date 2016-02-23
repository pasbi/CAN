#ifndef MERGETREEITEM_H
#define MERGETREEITEM_H

#include <QTreeWidgetItem>

struct MergeInfo
{
public:
    enum Type { AddItemType, DeleteItemType, Other };
    enum Action { AddItemAction, DeleteItemAction, NoAction };

    MergeInfo(Type type, const void* dataPointer);

    Type type() const { return m_type; }
    Action action() const;
    void setAction(Action a) { m_action = a; }
    const void* dataPointer() const { return m_dataptr; }

    MergeInfo& operator=(const MergeInfo& s);

private:
    Type m_type;
    Action m_action;

    // holds the pointer of the slave/master song/event/attachment
   const void* const m_dataptr;

};

class MergeTreeItem : public QTreeWidgetItem
{
public:

    MergeTreeItem(MergeInfo::Type type, const void* dataPointer, const QString& label);
    MergeTreeItem();

    QList<MergeTreeItem*> children() const { return m_children; }
    void addChild(MergeTreeItem *child);
    void setAction(MergeInfo::Action action);

    MergeInfo mergeInfo() { return m_mergeInfo; }

private:
    QList<MergeTreeItem*> m_children;
    MergeInfo m_mergeInfo;



};

#endif // MERGETREEITEM_H
