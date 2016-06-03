#ifndef DATABASEITEM_H
#define DATABASEITEM_H

#include <QObject>
#include "map.h"
#include "type_traits"
#include "databaseitembase.h"

template<typename T> class DatabaseItem : public DatabaseItemBase
{
protected:
    DatabaseItem(Database<T>* database);
    virtual ~DatabaseItem();

public:
    virtual void setDatabase(Database<T>* database);
    virtual Database<T>* database() const;
    T* copy() const;

    static QMultiMap<double, QPair<T*, T*>> sortSimilar(QList<T*> masterItems, QList<T*> slaveItems);
    static Ratio similarMapToRatio(const QMultiMap<double, QPair<T*, T*>>& map, double threshold);

private:
    Database<T>* m_database;
};


#endif // DATABASEITEM_H
