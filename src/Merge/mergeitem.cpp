#include "mergeitem.h"

MergeItemBase::MergeItemBase(Origin origin, Type type, void *pointer, const QString &baseLabel) :
    m_type(type),
    m_origin(origin),
    m_pointer(pointer),
    m_masterPointer(nullptr),
    m_slavePointer(nullptr),
    m_baseLabel(baseLabel)
{
    switch (origin)
    {
    case MasterProject:
        m_action = AddItemAction;
        break;
    case SlaveProject:
        m_action = DeleteItemAction;
        break;
    case BothProjects:
        m_action = ModifyItemAction;
        break;
    default:
        m_action = NoAction;
    }
}

MergeItemBase::MergeItemBase(const QByteArray &data)
{
    QDataStream stream(data);
    stream >> *this;
}

MergeItemBase::MergeItemBase() :
    MergeItemBase(NoProject, NoType, nullptr, "")
{
}

bool MergeItemBase::operator==(const MergeItemBase& other) const
{
    if (       m_action == other.m_action
            && m_type == other.m_type
            && m_origin == other.m_origin )
    {
        switch (m_type)
        {
        case MasterProject:
        case SlaveProject:
            return m_pointer == other.m_pointer
                    && m_baseLabel == other.m_baseLabel;
        case BothProjects:
            return m_masterPointer == other.m_masterPointer
                    && m_slavePointer == other.m_slavePointer
                    && m_masterBaseLabel == other.m_masterBaseLabel
                    && m_slaveBaseLabel == other.m_slaveBaseLabel;
        case NoProject:
            return true;
        }
    }

    return false;
}

QDataStream& operator<<(QDataStream& out, const MergeItemBase& mergeItem)
{
    out << static_cast<qint32>(mergeItem.m_action)
        << static_cast<qint32>(mergeItem.m_type)
        << static_cast<qint32>(mergeItem.m_origin)
        << reinterpret_cast<quintptr>(mergeItem.m_pointer)
        << reinterpret_cast<quintptr>(mergeItem.m_masterPointer)
        << reinterpret_cast<quintptr>(mergeItem.m_slavePointer)
        << mergeItem.m_baseLabel
        << mergeItem.m_masterBaseLabel
        << mergeItem.m_slaveBaseLabel;
    return out;
}

QDataStream& operator>>(QDataStream& in, MergeItemBase& mergeInfo)
{

    qint32 action, type, origin;
    in >> action >> type >> origin;
    mergeInfo.m_action = static_cast<MergeItemBase::Action>(action);
    mergeInfo.m_type = static_cast<MergeItemBase::Type>(type);
    mergeInfo.m_origin = static_cast<MergeItemBase::Origin>(origin);

    quintptr pointer, masterPointer, slavePointer;
    in >> pointer >> masterPointer >> slavePointer;
    mergeInfo.m_pointer = reinterpret_cast<void*>(pointer);
    mergeInfo.m_masterPointer = reinterpret_cast<void*>(masterPointer);
    mergeInfo.m_slavePointer = reinterpret_cast<void*>(slavePointer);

    in >> mergeInfo.m_baseLabel >> mergeInfo.m_masterBaseLabel >> mergeInfo.m_slaveBaseLabel;

    return in;
}

MergeItemBase::Action MergeItemBase::action() const
{
    if (origin() == MasterProject)
    {
        if (m_action == AddItemAction)
        {
            // master already contains this item
            return NoAction;
        }
        else if (m_action == DeleteItemAction)
        {
            // this item has to be deleted from master
            return DeleteItemAction;
        }
        Q_UNREACHABLE();
    }
    else if (origin() == SlaveProject)
    {
        if (m_action == AddItemAction)
        {
            // this item has to be added to master
            return AddItemAction;
        }
        else if (m_action == DeleteItemAction)
        {
            // master never contained this item, so do nothing.
            return NoAction;
        }
        Q_UNREACHABLE();
    }
    else if (origin() == BothProjects)
    {
        if (m_action == ModifyItemAction)
        {
            // i.e. neither keep master nor slave object, but keep combination of both.
            // how this combination looks will be specified somewhere else.
            return ModifyItemAction;
        }
        // there should not be any other option than how to combine them.
        Q_UNREACHABLE();
    }
    Q_UNREACHABLE();
    return NoAction;
}

void MergeItemBase::setAction(Action a)
{
    m_action = a;
}

void MergeItemBase::setPointer(void* dataPointer)
{
    m_pointer = dataPointer;
    m_masterPointer = nullptr;
    m_slavePointer = nullptr;
}

void MergeItemBase::setMasterSlavePointer(void* masterPointer, void* slavePointer)
{
    m_masterPointer = masterPointer;
    m_slavePointer = slavePointer;
    m_pointer = nullptr;
}

void MergeItemBase::setMasterSlaveLabel(const QString& masterLabel, const QString& slaveLabel)
{
    m_masterBaseLabel = masterLabel;
    m_slaveBaseLabel = slaveLabel;
    m_baseLabel = "";
}

QString MergeItemBase::label() const
{
    if (origin() == BothProjects)
    {
        return QString(QObject::tr("Modified: %2 -> %3")).arg(masterBaseLabel(), slaveBaseLabel());
    }
    else if (origin() == SlaveProject)
    {
        return QString(QObject::tr("Added: %2")).arg(baseLabel());
    }
    else if (origin() == MasterProject)
    {
        return QString(QObject::tr("Deleted: %2")).arg(baseLabel());
    }

    Q_UNREACHABLE();
    return "";
}

void MergeItemBase::setOrigin(Origin origin)
{
    m_origin = origin;
}

void MergeItemBase::setLabel(const QString &label)
{
    m_baseLabel = label;
    m_masterBaseLabel = "";
    m_slaveBaseLabel = "";
}

