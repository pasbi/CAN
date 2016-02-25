#ifndef DATABASEMERGER_H
#define DATABASEMERGER_H

#include "Database/database.h"
#include "Database/EventDatabase/event.h"
#include "Database/SongDatabase/song.h"
#include "mergeitem.h"


class DatabaseMergerPrivate
{
public:
    enum CompareResult { Unequal, Equal, Similar };
};

template<class T> class MergeTreeItem;

template<class T>
class DatabaseMerger
{
public:
    DatabaseMerger(Database<T>* master, Database<T>* slave)
    {
        m_master = master;
        m_slave = slave;
    }

    QList<MergeItemBase> createItems() const;

    Database<T>* master() const { return m_master; }
    Database<T>* slave() const { return m_slave; }

private:
    Database<T>* m_master;
    Database<T>* m_slave;
};


#endif // DATABASEMERGER_H
