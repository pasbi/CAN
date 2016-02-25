#ifndef MERGEITEM_H
#define MERGEITEM_H

#include <type_traits>
#include "Database/SongDatabase/song.h"
#include "Database/EventDatabase/event.h"
#include "Attachments/attachment.h"

class MergeItemBase
{
public:
    enum Type { SongType, EventType, AttachmentType, NoType };
    enum Origin { MasterProject, SlaveProject, BothProjects, NoProject };
    enum Action { AddItemAction, DeleteItemAction, ModifyItemAction, NoAction };
    MergeItemBase(const QByteArray& data);
    MergeItemBase();

protected:
    // use this constructor only via template<class T> MergeItem
    MergeItemBase(Origin origin, Type type, void* pointer, const QString& baseLabel);


public:
    QString label() const;

    void setMasterSlavePointer(void* masterPointer, void* slavePointer);
    void* masterPointer() const { return m_masterPointer; }
    void* slavePointer() const { return m_slavePointer; }
    void setPointer(void* pointer);
    void* pointer() const { return m_pointer; }


    void setLabel(const QString& label);
    QString baseLabel() const { return m_baseLabel; }

    void setMasterSlaveLabel(const QString& masterBaseLabel, const QString& slaveBaseLabel);
    QString masterBaseLabel() const { return m_masterBaseLabel; }
    QString slaveBaseLabel() const { return m_slaveBaseLabel; }

    Type type() const { return m_type; }

    void setAction(Action action);
    Action action() const;

    void setOrigin(Origin origin);
    Origin origin() const { return m_origin; }

    bool operator==(const MergeItemBase& other) const;


private:
    Type m_type;
    Action m_action;
    Origin m_origin;

    void* m_pointer;
    void* m_masterPointer;
    void* m_slavePointer;
    QString m_baseLabel;
    QString m_masterBaseLabel;
    QString m_slaveBaseLabel;


    friend QDataStream& operator<<(QDataStream& out, const MergeItemBase& mergeInfo);
    friend QDataStream& operator>>(QDataStream& in, MergeItemBase& mergeInfo);
};

QDataStream& operator<<(QDataStream& out, const MergeItemBase& mergeInfo);
QDataStream& operator>>(QDataStream& in, MergeItemBase& mergeInfo);

template<class T>
MergeItemBase::Type findType()
{
    if (std::is_same<T, Song>())
    {
        return MergeItemBase::SongType;
    }
    else if (std::is_same<T, Event>())
    {
        return MergeItemBase::EventType;
    }
    else if (std::is_same<T, Attachment>())
    {
        return MergeItemBase::AttachmentType;
    }

    Q_UNREACHABLE();
    return MergeItemBase::SongType; // return dummy
}

template<class T>
class MergeItem : public MergeItemBase
{
public:

    MergeItem(Origin origin, T *dataPointer, const QString& baseLabel) :
        MergeItemBase(origin, findType<T>(), dataPointer, baseLabel)
    {
    }

    MergeItem() :
        MergeItemBase()
    {

    }

    T* pointer() const { return static_cast<T*>(MergeItemBase::pointer()); }
    T* masterPointer() const { return static_cast<T*>(MergeItemBase::masterPointer()); }
    T* slavePointer() const { return static_cast<T*>(MergeItemBase::slavePointer()); }
    void setMasterSlavePointer(T* masterPointer, T* slavePointer)
    {
        MergeItemBase::setMasterSlavePointer(masterPointer, slavePointer);
    }

    void setPointer(T* pointer)
    {
        MergeItemBase::setPointer(pointer);
    }


};

#endif // MERGEITEM_H
