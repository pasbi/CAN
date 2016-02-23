#include "mergetreeitem.h"

MergeInfo::MergeInfo(Type type, const void *dataPointer) :
    m_type(type),
    m_action(NoAction),
    m_dataptr(dataPointer)
{

}

MergeTreeItem::MergeTreeItem(MergeInfo::Type type, const void *dataPointer, const QString &label) :
    QTreeWidgetItem({label}),
    m_mergeInfo(type, dataPointer)
{
}

MergeTreeItem::MergeTreeItem() :
    QTreeWidgetItem(),
    m_mergeInfo(MergeInfo::Other, nullptr)
{
}

void MergeTreeItem::addChild(MergeTreeItem *child)
{
    //TODO children-list is duplicate of QTreeWidgetItem::children
    m_children << child;
    QTreeWidgetItem::addChild(child);
}

void MergeTreeItem::setAction(MergeInfo::Action action)
{
    m_mergeInfo.setAction(action);
}

MergeInfo::Action MergeInfo::action() const
{
    if (m_type == DeleteItemType)
    {
        if (m_action == AddItemAction)
        {
            // master already contains this item
            return MergeInfo::NoAction;
        }
        else if (m_action == DeleteItemAction)
        {
            // this item has to be deleted from master
            return MergeInfo::DeleteItemAction;
        }
        Q_UNREACHABLE();
    }
    else if (m_type == AddItemType)
    {
        if (m_action == AddItemAction)
        {
            // this item has to be added to master
            return MergeInfo::AddItemAction;
        }
        else if (m_action == DeleteItemAction)
        {
            // master never contained this item, so do nothing.
            return MergeInfo::NoAction;
        }
    }
    Q_UNREACHABLE();
    return MergeInfo::NoAction;
}
