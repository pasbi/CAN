#ifndef MERGETREEITEM_H
#define MERGETREEITEM_H

#include <type_traits>
#include <QTreeWidgetItem>
#include "Database/SongDatabase/song.h"
#include "Database/EventDatabase/event.h"
#include "Attachments/attachment.h"

class MergeInfoBase
{
public:
    enum Type { AddItemType, DeleteItemType, ModifyItemType, Other };
    enum Action { AddItemAction, DeleteItemAction, ModifyItemAction, NoAction };

    MergeInfoBase(Type type, void* dataPointer);
    Type type() const;
    Action action() const;
    void setAction(Action a);

    void setDataPointer(void *dataPointer);
    void *dataPointer() const;

    void setMasterSlavePointer(void *masterPointer, void *slavePointer);
    void* masterPointer() const;
    void* slavePointer() const;

private:
    Type m_type;
    Action m_action;

    // use this for delete and add action
    void* m_dataPtr;

    // use this for modification
    void* m_masterPtr;
    void* m_slavePtr;

    friend QDataStream& operator<<(QDataStream& out, const MergeInfoBase& mergeInfo);
    friend QDataStream& operator>>(QDataStream& in, MergeInfoBase& mergeInfo);
};

QDataStream& operator<<(QDataStream& out, const MergeInfoBase& mergeInfo);
QDataStream& operator>>(QDataStream& in, MergeInfoBase& mergeInfo);

template<class T>
class MergeInfo : public MergeInfoBase
{
public:
    MergeInfo(Type type, T *dataPointer) :
        MergeInfoBase(type, dataPointer),
        m_dataptr(dataPointer)
    {

    }

    const T* dataPointer() const { return m_dataptr; }

private:
    // holds the pointer of the slave/master song/event/attachment
   const T* m_dataptr;

};


class MergeTreeItemBase : public QTreeWidgetItem
{
public:
    enum Type { SongType, EventType, AttachmentType };

protected:
    MergeTreeItemBase(Type type, const QString& baseLabel);
    MergeTreeItemBase();


public:
    void setAction(MergeInfoBase::Action action);
    MergeInfoBase *mergeInfoBase();
    const MergeInfoBase *mergeInfoBase() const;
    Type type() const { return m_type; }
    void updateLabel();

private:
    Type m_type;
    QString m_baseLabel;
};

template<class T>
MergeTreeItemBase::Type findType()
{
    if (std::is_same<T, Song>())
    {
        return MergeTreeItemBase::SongType;
    }
    else if (std::is_same<T, Event>())
    {
        return MergeTreeItemBase::EventType;
    }
    else if (std::is_same<T, Attachment>())
    {
        return MergeTreeItemBase::AttachmentType;
    }

    Q_UNREACHABLE();
    return MergeTreeItemBase::SongType; // return dummy
}

template<class T>
class MergeTreeItem : public MergeTreeItemBase
{
public:

    MergeTreeItem(MergeInfoBase::Type type, T *dataPointer, const QString& baseLabel) :
        MergeTreeItemBase(findType<T>(), baseLabel),
        m_mergeInfo(type, dataPointer)
    {
        updateLabel();
    }

    MergeTreeItem() :
        MergeTreeItemBase(),
        m_mergeInfo(MergeInfoBase::Other, nullptr)
    {

    }

    QList<MergeTreeItem*> children() const
    {
        return m_children;
    }

    void addChild(MergeTreeItem *child)
    {
        //TODO children-list is duplicate of QTreeWidgetItem::children
        m_children << child;
        QTreeWidgetItem::addChild(child);
    }

    MergeInfo<T>* mergeInfo()
    {
        return &m_mergeInfo;
    }

    const MergeInfo<T>* mergeInfo() const
    {
        return &m_mergeInfo;
    }


private:
    QList<MergeTreeItem*> m_children;
    MergeInfo<T> m_mergeInfo;



};

#endif // MERGETREEITEM_H
