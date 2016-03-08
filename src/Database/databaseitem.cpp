#include "databaseitem.h"

DatabaseItemBase::DatabaseItemBase(const QStringList &attributeKeys) :
    m_attributes(attributeKeys)
{

}

void DatabaseItemBase::setAttribute(const QString &key, const QVariant &value)
{
    m_attributes.set(key, value);
}

QVariant DatabaseItemBase::attribute(const QString& key) const
{
    return m_attributes[key];
}

QStringList DatabaseItemBase::attributeKeys() const
{
    return m_attributes.keys();
}


void DatabaseItemBase::deserialize(QDataStream & in)
{
    PersistentObject::deserialize(in);

    in >> m_attributes;
}

void DatabaseItemBase::serialize(QDataStream & out) const
{
    PersistentObject::serialize(out);
    PedanticVariantMap copy = m_attributes;
    for (const QString& key : skipSerializeAttributes())
    {
        copy.take(key);
    }
    out << copy;
}

bool DatabaseItemBase::canRemove() const
{
    return true;
}
