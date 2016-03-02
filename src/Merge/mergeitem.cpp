#include "mergeitem.h"
#include "Database/databaseitem.h"
#include <QObject>

MergeItem::MergeItem(DatabaseItemBase* master, DatabaseItemBase* slave) :
    m_type(Modify),
    m_action(ModifyAction),
    m_master(master),
    m_slave(slave)
{
    setupModifyDetails();
}

MergeItem::MergeItem(DatabaseItemBase* item, Type type, Action action) :
    m_type(type),
    m_action(action),
    m_master(nullptr),
    m_slave(nullptr)
{
    Q_ASSERT(action != ModifyAction);

    switch (type)
    {
    case Add:
        m_slave = item;
        break;
    case Remove:
        m_master = item;
        break;
    case Modify:
    default:
        // use othe constructor!
        Q_UNREACHABLE();
        break;
    }
}

DatabaseItemBase* MergeItem::master() const
{
    Q_ASSERT(type() == Remove || type() == Modify);
    return m_master;
}

DatabaseItemBase* MergeItem::slave() const
{
    Q_ASSERT(type() == Add || type() == Modify);
    return m_slave;
}


MergeItem::Type MergeItem::type() const
{
    return m_type;
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
    switch (type())
    {
    case Add:
        return slave()->label();
    case Remove:
        return master()->label();
    case Modify:
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
            qDebug() << "add modify detail " << key;
            m_modifyDetails << ModifyDetail(key, preference<MergeItem::Decision>("defaultActionMergeModify"));
        }
    }
}


DEFINE_ENUM_STREAM_OPERATORS(MergeItem::Action)
DEFINE_ENUM_STREAM_OPERATORS(MergeItem::Decision)

