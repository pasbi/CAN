#include "databaseitembase.h"
#include "global.h"

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


bool DatabaseItemBase::canRemove() const
{
    return true;
}

void DatabaseItemBase::serialize(QDataStream &out) const
{
    PedanticVariantMap copy = m_attributes;
    for (const QString& key : skipSerializeAttributes())
    {
        copy.take(key);
    }
    qDebug() << "DatabaseItemBase:" << copy.keys() << copy;
    out << copy;
}

void DatabaseItemBase::deserialize(QDataStream &in)
{
    in >> m_attributes;
}


QDataStream& operator<<(QDataStream& out, const DatabaseItemBase* item)
{
    item->serialize(out);
    return out;
}

QDataStream& operator>>(QDataStream& in, DatabaseItemBase* item)
{
    item->deserialize(in);
    return in;
}

bool DatabaseItemBase::operator ==(const DatabaseItemBase& other) const
{
    QByteArray a, b;
    QDataStream aStream(&a, QIODevice::WriteOnly);
    QDataStream bStream(&b, QIODevice::WriteOnly);

    aStream << this;
    bStream << &other;

    qDebug() << ".......................";
    qDebug() << "a: " << a.toHex();
    qDebug() << "b: " << b.toHex();

    return a == b;
}
