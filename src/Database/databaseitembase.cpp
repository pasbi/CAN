#include "databaseitembase.h"
#include "global.h"
#include "jarowinkler.h"
#include <qmath.h>

DatabaseItemBase::DatabaseItemBase()
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

void DatabaseItemBase::addAttributeKey(const QString &key)
{
    m_attributes.addKey(key);
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

    if (QString(metaObject()->className()) != QString(other.metaObject()->className()))
    {
        return false;
    }

    QByteArray a, b;
    QDataStream aStream(&a, QIODevice::WriteOnly);
    QDataStream bStream(&b, QIODevice::WriteOnly);

    aStream << this;
    bStream << &other;

    return a == b;
}

bool DatabaseItemBase::operator !=(const DatabaseItemBase& other) const
{
    return !(*this == other);
}

DatabaseItemBase::Ratio operator+(DatabaseItemBase::Ratio& a, DatabaseItemBase::Ratio& b)
{
    return DatabaseItemBase::Ratio(a.d + b.d, a.n + b.n);
}


DatabaseItemBase::Ratio DatabaseItemBase::similarity(const DatabaseItemBase *other) const
{
    if (QString(metaObject()->className()) != QString(other->metaObject()->className()))
    {
        return Ratio();
    }

    Ratio r;
    for (const QString& key : attributeKeys())
    {
        Q_ASSERT(other->attributeKeys().contains(key));
        QVariant thisAttribute = attribute(key);
        QVariant otherAttribute = other->attribute(key);
        if (thisAttribute.canConvert<QString>() && otherAttribute.canConvert<QString>())
        {
            r.d += jaro_winkler_distance(thisAttribute.toString(), otherAttribute.toString());
        }
        else
        {
            if (thisAttribute == otherAttribute)
            {
                r.d += 1;
            }
            else
            {
                r.d += 0;
            }
        }
        r.n += 1;
    }

    return r;
}

DatabaseItemBase::Ratio::Ratio(double d, double n)
    : d(d)
    , n(n)
{

}

DatabaseItemBase::Ratio::operator double()
{
    double ratio = d/n;
    Q_ASSERT(ratio >= 0.0 && ratio <= 1.0);

    // n can be interpreted as confidence level,
    // d/n ~ similarity.
    // make the ratio more extrem if the confidence is greater.
    if (ratio < 0.5)
    {
        ratio = qPow(ratio * 2.0, n)/2.0;
    }
    else
    {
        ratio = 1.0 - qPow((1.0 - ratio) * 2, n)/2.0;
    }

    return ratio;
}

DatabaseItemBase::Ratio& DatabaseItemBase::Ratio::operator+=(const Ratio& other)
{
    d += other.d;
    n += other.n;
    return (*this);
}

QDebug& operator<<(QDebug& out, const DatabaseItemBase::Ratio& ratio)
{
    out << QString("Ratio(%1, %2)").arg(ratio.d).arg(ratio.n).toStdString().c_str();
    return out;
}
