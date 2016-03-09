#ifndef DATABASEITEM_H
#define DATABASEITEM_H

#include <QObject>
#include "map.h"
#include "type_traits"


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

template<typename T> class DatabaseItem : public DatabaseItemBase
{
protected:
    DatabaseItem(const QStringList& attributeKeys, Database<T>* database) :
        DatabaseItemBase(attributeKeys),
        m_database(database)
    {

    }

    virtual ~DatabaseItem()
    {

    }


public:
    virtual void setDatabase(Database<T>* database)
    {
        m_database = database;
    }

    virtual Database<T>* database() const
    {
        return m_database;
    }

    T* copy() const
    {
        QByteArray buffer;
        QDataStream writeStream(&buffer, QIODevice::WriteOnly);
        writeStream << this;

        QDataStream readStream(buffer);
        T* copy = new T(database());
        readStream >> copy;

        return copy;
    }

private:
    Database<T>* m_database;
};


#endif // DATABASEITEM_H
