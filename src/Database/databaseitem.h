#ifndef DATABASEITEM_H
#define DATABASEITEM_H

#include "taggable.h"
#include <QObject>
#include <QBuffer>

template<typename T> class Database;
template<typename T>
class DatabaseItem : public QObject, public Taggable
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
    Database<T>* database() const { return m_database; }

    T* copy(Database<T>* database) const
    {
        QBuffer buffer;
        assert(buffer.open(QIODevice::ReadWrite));
        QDataStream stream(&buffer);
        stream << this;

        T* copy = new T(database);
        stream >> copy;
        return copy;
    }

    virtual QStringList textAttributes() const = 0;

private:
    Database<T>* m_database;
};


#endif // DATABASEITEM_H
