#ifndef DATABASEITEM_H
#define DATABASEITEM_H

#include "persistentobject.h"
#include <QObject>
#include "pedanticmap.h"
#include <QBuffer> //TODO

class DatabaseItemBase : public QObject, public PersistentObject
{
protected:
    explicit DatabaseItemBase(const QStringList& attributeKeys);

public:
    QVariant attribute(const QString& key) const;
    void setAttribute(const QString& key, const QVariant& value);
    virtual QString attributeDisplay(const QString& key) const = 0;
    QStringList attributeKeys() const;

protected:
    virtual QStringList skipSerializeAttributes() const { return QStringList(); }

protected:
    virtual void deserialize(QDataStream & in);
    virtual void serialize(QDataStream & out) const;

private:
    PedanticVariantMap m_attributes;
};

template<typename T> class Database;

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
    virtual Database<T>* database() const { return m_database; }

    T* copy() const
    {
        return copy(database());
    }

    T* copy(Database<T>* database) const
    {
        //TODO replace buffer with QByteArray?
        QBuffer buffer;
        assert(buffer.open(QIODevice::ReadWrite));

        QDataStream stream(&buffer);
        stream << this;

        buffer.reset();
        T* copy = new T(database);
        stream >> copy;

        return copy;
    }

private:
    Database<T>* m_database;
};


#endif // DATABASEITEM_H
