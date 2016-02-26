#ifndef MERGEITEM_H
#define MERGEITEM_H

#include <type_traits>
#include "Database/SongDatabase/song.h"
#include "Database/EventDatabase/event.h"
#include "Attachments/attachment.h"
#include "global.h"

//TODO disable copy, use one list of merge items in Merge class and make them easily accessible (e.g. static)
// everywhere else, use only pointers to MergeItemBase objects that are stored in Merge class.
class MergeItemBase
{
public:
    enum Type { SongType, EventType, AttachmentType, NoType };
    enum Origin { MasterProject, SlaveProject, BothProjects, NoProject };
    enum Action { AddItemAction, DeleteItemAction, ModifyItemAction, NoAction };
    MergeItemBase();
    MergeItemBase(Origin origin, Type type, void* pointer, const QString& baseLabel);
    ~MergeItemBase();

    MergeItemBase(const MergeItemBase &data) = delete;
    const MergeItemBase& operator=(const MergeItemBase& other) = delete;

    QString label() const;
    void setMasterSlavePointer(void* masterPointer, void* slavePointer);
    template<class T = void> T* masterPointer() const { return static_cast<T*>(m_masterPointer); }
    template<class T = void> T* slavePointer() const { return static_cast<T*>(m_masterPointer); }
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

    /**
     * @brief setCombinationPointer remember to delete combination pointer before overwriting it!
     * @param combinationPointer
     */
    void setCombinationPointer(void* combinationPointer);
    // pure virtual is not possible for technical reasons
    void initializeCombinationObject();
    void deleteCombinationObject();
    template<class T = void> T* combinationPointer() const { return static_cast<T*>(m_combinationPointer); }

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

    // this field is not serialized or compared.
    void* m_combinationPointer;
};

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
    T* combinationPointer() const { return static_cast<T*>(MergeItemBase::combinationPointer()); }
    void setMasterSlavePointer(T* masterPointer, T* slavePointer)
    {
        MergeItemBase::setMasterSlavePointer(masterPointer, slavePointer);
    }

    void setPointer(T* pointer)
    {
        MergeItemBase::setPointer(pointer);
    }

    void initializeCombinationObject()
    {
        setCombinationPointer(masterPointer()->copy());
    }

    void deleteCombinationObject()
    {
        delete combinationPointer();
        setCombinationPointer(nullptr);
    }


};

#endif // MERGEITEM_H
