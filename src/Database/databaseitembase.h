#ifndef DATABASEITEMBASE_H
#define DATABASEITEMBASE_H

#include <QVariant>
#include "map.h"


template<typename T> class Database;
class DatabaseBase;

class DatabaseItemBase
{
protected:
    explicit DatabaseItemBase(const QStringList& attributeKeys);

public:
    QVariant attribute(const QString& key) const;
    void setAttribute(const QString& key, const QVariant& value);
    virtual QString attributeDisplay(const QString& key) const = 0;
    QStringList attributeKeys() const;
    virtual QString label() const = 0;

    virtual bool canRemove() const;

    virtual void serialize(QDataStream& out) const;
    virtual void deserialize(QDataStream& in);


protected:
    virtual QStringList skipSerializeAttributes() const { return QStringList(); }

private:
    PedanticVariantMap m_attributes;

    friend QDataStream& operator<<(QDataStream& out, const DatabaseItemBase* item);
    friend QDataStream& operator>>(QDataStream& in, DatabaseItemBase* item);
};

QDataStream& operator<<(QDataStream& out, const DatabaseItemBase* item);
QDataStream& operator>>(QDataStream& in, DatabaseItemBase* item);

#endif // DATABASEITEMBASE_H