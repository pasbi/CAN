#ifndef DATABASEITEMBASE_H
#define DATABASEITEMBASE_H

#include <QVariant>
#include "map.h"

#define SIMILARITY_BEGIN_CHECK(T) \
    if (!other || !other->inherits(staticMetaObject.className())) \
    { \
        return Ratio(); \
    } \
    const T* other##T = static_cast<const T*>(other);



template<typename T> class Database;
class DatabaseBase;

class DatabaseItemBase : public QObject
{
protected:
    explicit DatabaseItemBase();

public:
    // they are virtual, since it may be inconvenient/impossible to store some attributes in the
    // map. Instead, store them as a explicit member and overwrite attribute() and setAttribute()
    virtual QVariant attribute(const QString& key) const;
    virtual void setAttribute(const QString& key, const QVariant& value);

    virtual QString attributeDisplay(const QString& key) const = 0;
    QStringList attributeKeys() const;
    virtual QString label() const = 0;

    virtual bool canRemove() const;

    virtual void serialize(QDataStream& out) const;
    virtual void deserialize(QDataStream& in);

    bool operator==(const DatabaseItemBase& other) const;
    bool operator!=(const DatabaseItemBase& other) const;

    struct Ratio
    {
        Ratio(double d = 0, double n = 0);
        double d, n;
        operator double();
        Ratio& operator+=(const Ratio& other);
    };

    virtual Ratio similarity(const DatabaseItemBase* other) const;

protected:
    virtual QStringList skipSerializeAttributes() const { return {}; }
    void addAttributeKey(const QString& key);

private:
    PedanticVariantMap m_attributes;

    friend QDataStream& operator<<(QDataStream& out, const DatabaseItemBase* item);
    friend QDataStream& operator>>(QDataStream& in, DatabaseItemBase* item);
};

QDataStream& operator<<(QDataStream& out, const DatabaseItemBase* item);
QDataStream& operator>>(QDataStream& in, DatabaseItemBase* item);
DatabaseItemBase::Ratio operator+(DatabaseItemBase::Ratio& a, DatabaseItemBase::Ratio& b);


#endif // DATABASEITEMBASE_H
