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


DEFINE_ENUM_STREAM_OPERATORS(MergeItem::Action)
DEFINE_ENUM_STREAM_OPERATORS(MergeItem::Decision)

