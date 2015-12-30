#ifndef DATABASEITEM_H
#define DATABASEITEM_H

#include "persistentobject.h"
#include <QObject>
#include <QBuffer>

template<typename T> class Database;
template<typename T>
class DatabaseItem : public QObject, public PersistentObject
{
protected:
    DatabaseItem(Database<T>* database) :
        m_database(database)
    {

    }

    virtual ~DatabaseItem()
    {

    }

public:
    virtual Database<T>* database() const { return m_database; }

    T* copy(Database<T>* database) const
    {
        QBuffer buffer;
        assert(buffer.open(QIODevice::ReadWrite));

        QDataStream stream(&buffer);
        stream << this;

        buffer.reset();
        T* copy = new T(database);
        stream >> copy;

        return copy;
    }

    virtual QStringList textAttributes() const = 0;

private:
    Database<T>* m_database;
};


#endif // DATABASEITEM_H
