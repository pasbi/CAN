#include "mergeitem.h"

MergeItemBase::MergeItemBase(Origin origin, Type type, void *pointer, const QString &baseLabel) :
    m_type(type),
    m_origin(origin),
    m_pointer(pointer),
    m_masterPointer(nullptr),
    m_slavePointer(nullptr),
    m_baseLabel(baseLabel),
    m_combinationPointer(nullptr)
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

MergeItemBase::~MergeItemBase()
{
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
}

void MergeItemBase::setMasterSlavePointer(void* masterPointer, void* slavePointer)
{
    m_masterPointer = masterPointer;
    m_slavePointer = slavePointer;
}

void MergeItemBase::setMasterSlaveLabel(const QString& masterLabel, const QString& slaveLabel)
{
    m_masterBaseLabel = masterLabel;
    m_slaveBaseLabel = slaveLabel;
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
}

void MergeItemBase::setCombinationPointer(void *combinationPointer)
{
    m_combinationPointer = combinationPointer;
}

void MergeItemBase::initializeCombinationObject()
{
    switch (type())
    {
    case SongType:
        setCombinationPointer(masterPointer<Song>()->copy());
        break;
    case EventType:
        setCombinationPointer(masterPointer<Event>()->copy());
        break;
    case AttachmentType:
        setCombinationPointer(masterPointer<Attachment>()->copy());
        break;
    default:
        Q_UNREACHABLE();
    }
}

void MergeItemBase::deleteCombinationObject()
{
    if (m_combinationPointer)
    {
        //TODO ??
    }
    switch (type())
    {
    case SongType:
        delete combinationPointer<Song>();
        break;
    case EventType:
        delete combinationPointer<Event>();
        break;
    case AttachmentType:
        delete combinationPointer<Attachment>();
        break;
    default:
        Q_ASSERT(m_combinationPointer == nullptr);
    }
    setCombinationPointer(nullptr);
}

