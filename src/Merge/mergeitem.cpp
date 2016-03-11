#include "mergeitem.h"
#include "Database/databaseitem.h"
#include <QObject>

MergeItem::MergeItem(DatabaseItemBase* master, DatabaseItemBase* slave, Action action) :
    m_origin(Both),
    m_action(action),
    m_master(master),
    m_slave(slave)
{
    setupModifyDetails();
}

MergeItem::~MergeItem()
{

}

MergeItem::MergeItem(DatabaseItemBase* item, Origin origin, Action action) :
    m_origin(origin),
    m_action(action),
    m_master(nullptr),
    m_slave(nullptr)
{
    Q_ASSERT(action != ModifyAction);

    switch (origin)
    {
    case Slave:
        m_slave = item;
        break;
    case Master:
        m_master = item;
        break;
    case Both:
    default:
        // use othe constructor!
        Q_UNREACHABLE();
        break;
    }
}

DatabaseItemBase* MergeItem::master() const
{
    Q_ASSERT(origin() & Master);
    return m_master;
}

DatabaseItemBase* MergeItem::slave() const
{
    Q_ASSERT(origin() & Slave);
    return m_slave;
}


MergeItem::Origin MergeItem::origin() const
{
    return m_origin;
}

MergeItem::Action MergeItem::action() const
{
    return m_action;
}

void MergeItem::setAction(Action action)
{
    // we are only allowed to change AddAction to RemoveAction and vice versa.222
    Q_ASSERT(m_action != ModifyAction);
    Q_ASSERT(action != ModifyAction);
    m_action = action;
}

QList<MergeItem::ModifyDetail> MergeItem::modifyDetails() const
{
    return m_modifyDetails;
}

// very custom QList::indexOf surrogate. We could do this with overriding MergeItem::ModifyDetail::operator==, but
// in this context, equality means having the same key. It is probably a bad idea to express this with the operator==.
int findModifyDetail(const QList<MergeItem::ModifyDetail>& list, const MergeItem::ModifyDetail& md)
{
    for (int i = 0; i < list.length(); ++i)
    {
        if (list[i].key() == md.key())
        {
            return i;
        }
    }
    return -1;
}

void MergeItem::updateModifyDetails(const QList<ModifyDetail> modifyDetails)
{
    for (const ModifyDetail& modifyDetail : modifyDetails)
    {
        int i = findModifyDetail(m_modifyDetails, modifyDetail);
        Q_ASSERT(i >= 0);   // assert modifyDetail is contained in m_modifyDetails
        m_modifyDetails.replace(i, modifyDetail);
    }
}

QString MergeItem::label() const
{
    switch (origin())
    {
    case Master:
        return master()->label();
    case Slave:
        return slave()->label();
    case Both:
        return QString("%1 -> %2").arg(master()->label(), slave()->label());
    default:
        Q_UNREACHABLE();
        return "";
    }
}

void MergeItem::setupModifyDetails()
{
    Q_ASSERT(master()->attributeKeys() == slave()->attributeKeys());
    for (const QString& key : master()->attributeKeys())
    {
        if (master()->attribute(key) != slave()->attribute(key))
        {
            m_modifyDetails << ModifyDetail(key, preference<MergeItem::Decision>("defaultActionMergeModify"));
        }
    }
}

void MergeItem::performModification() const
{
    Q_ASSERT(origin() == Both && action() == ModifyAction);
    for (const ModifyDetail& detail : m_modifyDetails)
    {
        const QString key = detail.key();
        QVariant value;
        switch (detail.decision())
        {
        case UseMaster:
            value = master()->attribute(key);
            break;
        case UseSlave:
            value = slave()->attribute(key);
            break;
        default:
            Q_UNREACHABLE();
        }

        master()->setAttribute(detail.key(), value);
    }
}

bool MergeItem::sortMasterSlaveItem(MergeItem *&masterItem, MergeItem *&slaveItem)
{
    const MergeItem* &masterItemNonConst = const_cast<const MergeItem*&>(masterItem);
    const MergeItem* &slaveItemNonconst = const_cast<const MergeItem*&>(slaveItem);
    return sortMasterSlaveItem(masterItemNonConst, slaveItemNonconst);
}


bool MergeItem::sortMasterSlaveItem(const MergeItem*& masterItem, const MergeItem*& slaveItem)
{
    if (masterItem->origin() == Master && slaveItem->origin() == Slave)
    {
        return true; // correct order
    }
    if (masterItem->origin() == Slave && slaveItem->origin() == Master)
    {
        qSwap(masterItem, slaveItem);
        return true; // correct order after swap
    }
    return false;   //two add or two remove types cannot be corrected.
}

bool MergeItem::canJoin(const MergeItem *other) const
{
    const MergeItem* masterItem = this;
    const MergeItem* slaveItem = other;
    if (!sortMasterSlaveItem(masterItem, slaveItem))
    {
        // we need exactly one master and one slave item
        return false;
    }
    else
    {
        QString masterClassname = masterItem->master()->metaObject()->className();
        QString slaveClassame = slaveItem->slave()->metaObject()->className();
        if (masterClassname != slaveClassame)
        {
            return false;
        }
        else
        {
            return true;
        }
    }
}

bool MergeItem::inherits(const QStringList& classnames) const
{
    Q_ASSERT(QString(master()->metaObject()->className()) == QString(slave()->metaObject()->className()));

    for (const QString& classname : classnames)
    {
        if (master()->inherits(classname.toStdString().c_str()))
        {
            return true;
        }
    }
    return false;
}


DEFINE_ENUM_STREAM_OPERATORS(MergeItem::Action)
DEFINE_ENUM_STREAM_OPERATORS(MergeItem::Decision)

