#include "mergetreeitem.h"

MergeInfoBase::MergeInfoBase(Type type, void* dataPointer) :
    m_type(type),
    m_action(NoAction),
    m_dataPtr(dataPointer)
{

}

QDataStream& operator<<(QDataStream& out, const MergeInfoBase& mergeInfo)
{
    out << static_cast<qint32>(mergeInfo.action())
        << static_cast<qint32>(mergeInfo.type())
        << reinterpret_cast<quintptr>(mergeInfo.dataPointer());
    return out;
}

QDataStream& operator>>(QDataStream& in, MergeInfoBase& mergeInfo)
{
    quintptr intptr;
    qint32 action, type;
    in >> action >> type >> intptr;
    mergeInfo.m_dataPtr = reinterpret_cast<void*>(intptr);
    mergeInfo.m_action = static_cast<MergeInfoBase::Action>(action);
    mergeInfo.m_type = static_cast<MergeInfoBase::Type>(type);
    return in;
}

MergeInfoBase::Type MergeInfoBase::type() const
{
    return m_type;
}

MergeInfoBase::Action MergeInfoBase::action() const
{
    if (m_type == DeleteItemType)
    {
        if (m_action == AddItemAction)
        {
            // master already contains this item
            return MergeInfoBase::NoAction;
        }
        else if (m_action == DeleteItemAction)
        {
            // this item has to be deleted from master
            return MergeInfoBase::DeleteItemAction;
        }
        Q_UNREACHABLE();
    }
    else if (m_type == AddItemType)
    {
        if (m_action == AddItemAction)
        {
            // this item has to be added to master
            return MergeInfoBase::AddItemAction;
        }
        else if (m_action == DeleteItemAction)
        {
            // master never contained this item, so do nothing.
            return MergeInfoBase::NoAction;
        }
    }
    else if (m_type == ModifyItemType)
    {
        if (m_action == ModifyItemAction)
        {
            // i.e. neither keep master nor slave object, but keep combination of both.
            // how this combination looks will be specified somewhere else.
            return MergeInfoBase::ModifyItemAction;
        }
    }
    Q_UNREACHABLE();
    return MergeInfoBase::NoAction;
}

void MergeInfoBase::setAction(Action a)
{
    m_action = a;
}

void* MergeInfoBase::dataPointer() const
{
    return m_dataPtr;
}

void MergeInfoBase::setDataPointer(void* dataPointer)
{
    m_dataPtr = dataPointer;
    m_masterPtr = nullptr;
    m_slavePtr = nullptr;
}

void MergeInfoBase::setMasterSlavePointer(void* masterPointer, void* slavePointer)
{
    m_masterPtr = masterPointer;
    m_slavePtr = slavePointer;
    m_dataPtr = nullptr;
    m_type = MergeInfoBase::ModifyItemType;
}

void* MergeInfoBase::masterPointer() const
{
    return m_masterPtr;
}

void* MergeInfoBase::slavePointer() const
{
    return m_slavePtr;
}

const MergeInfoBase* MergeTreeItemBase::mergeInfoBase() const
{
    switch (type())
    {
    case MergeTreeItemBase::SongType:
        return static_cast<const MergeTreeItem<Song>*>(this)->mergeInfo();
    case MergeTreeItemBase::EventType:
        return static_cast<const MergeTreeItem<Event>*>(this)->mergeInfo();
    case MergeTreeItemBase::AttachmentType:
        return static_cast<const MergeTreeItem<Attachment>*>(this)->mergeInfo();
    }
    Q_UNREACHABLE();
    return mergeInfoBase();
}

MergeInfoBase* MergeTreeItemBase::mergeInfoBase()
{
    switch (type())
    {
    case MergeTreeItemBase::SongType:
        return static_cast<MergeTreeItem<Song>*>(this)->mergeInfo();
    case MergeTreeItemBase::EventType:
        return static_cast<MergeTreeItem<Event>*>(this)->mergeInfo();
    case MergeTreeItemBase::AttachmentType:
        return static_cast<MergeTreeItem<Attachment>*>(this)->mergeInfo();
    }
    Q_UNREACHABLE();
    return mergeInfoBase();
}

void MergeTreeItemBase::setAction(MergeInfoBase::Action action)
{
    mergeInfoBase()->setAction(action);
}

void MergeTreeItemBase::updateLabel()
{
    QString label;
    qDebug() << mergeInfoBase()->type() << m_baseLabel;
    switch (mergeInfoBase()->type())
    {
    case MergeInfoBase::AddItemType:
        qDebug() << "added" << m_baseLabel;
        label = QObject::tr("Added");
        break;
    case MergeInfoBase::DeleteItemType:
        qDebug() << "deleted" << m_baseLabel;
        label = QObject::tr("Deleted");
        break;
    case MergeInfoBase::ModifyItemType:
        qDebug() << "modified" << m_baseLabel;
        label = QObject::tr("Modified");
        break;
    case MergeInfoBase::Other:
    default:
        label = "ROOT";
    }
    label = QString("%1: %2").arg(label).arg(m_baseLabel);
    setText(0, label);

}

MergeTreeItemBase::MergeTreeItemBase(Type type, const QString& baseLabel) :
    QTreeWidgetItem(),
    m_type(type),
    m_baseLabel(baseLabel)
{
}

MergeTreeItemBase::MergeTreeItemBase() :
    QTreeWidgetItem()
{
}
