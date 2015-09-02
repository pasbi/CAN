#ifndef DATABASEITEM_H
#define DATABASEITEM_H

#include "taggable.h"
#include <QObject>

template<typename T> class Database;
template<typename T>
class DatabaseItem : public QObject, public Taggable
{
public:
    DatabaseItem(Database<T>* database) :
        m_database(database)
    {

    }

    virtual ~DatabaseItem()
    {

    }

    Database<T>* database() const { return m_database; }

    T* copy(Database<T>* database) const
    {
        T* copy = new T(database);
        copy->restoreFromJsonObject(this->toJsonObject());
        return copy;
    }

private:
    Database<T>* m_database;
};

#endif // DATABASEITEM_H
