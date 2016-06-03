#include "databaseitem.h"
#include "Database/SongDatabase/song.h"
#include "Attachments/attachment.h"
#include "Database/EventDatabase/event.h"
#include "Database/EventDatabase/setlistitem.h"

template<class T> DatabaseItem<T>::DatabaseItem(Database<T>* database) :
    DatabaseItemBase(),
    m_database(database)
{

}

template<class T> DatabaseItem<T>::~DatabaseItem()
{

}


template<class T> void DatabaseItem<T>::setDatabase(Database<T>* database)
{
    m_database = database;
}

template<class T> Database<T>* DatabaseItem<T>::database() const
{
    return m_database;
}

template<class T>  T* DatabaseItem<T>::copy() const
{
    QByteArray buffer;
    QDataStream writeStream(&buffer, QIODevice::WriteOnly);
    writeStream << this;

    QDataStream readStream(buffer);
    T* copy = new T(database());
    readStream >> copy;

    return copy;
}

template<>  Attachment* DatabaseItem<Attachment>::copy() const
{
    QByteArray buffer;
    QDataStream writeStream(&buffer, QIODevice::WriteOnly);
    writeStream << this;

    QDataStream readStream(buffer);
    Attachment* copy = Attachment::create(static_cast<const Attachment*>(this)->type());
    readStream >> copy;

    return copy;
}


template<class T> QMultiMap<double, QPair<T*, T*>> DatabaseItem<T>::sortSimilar(QList<T*> masterItems, QList<T*> slaveItems)
{
    // sort the <master, slave> pairs by their similarity.

    QMultiMap<double, QPair<T*, T*>> ratioMasterSlaveMap;
    for (T* master : masterItems)
    {
        ratioMasterSlaveMap.insert(0, qMakePair(master, nullptr));
        for (T* slave : slaveItems)
        {
            Ratio sim = master->similarity(slave);
            ratioMasterSlaveMap.insert(sim, qMakePair(master, slave));
        }
    }
    for (T* slave : slaveItems)
    {
        ratioMasterSlaveMap.insert(0, qMakePair(nullptr, slave));
    }

    auto it = ratioMasterSlaveMap.end();
    while (it != ratioMasterSlaveMap.begin())
    {
        it--;

        T* master = it.value().first;
        T* slave = it.value().second;
        auto deleteIt = ratioMasterSlaveMap.begin();
        while (deleteIt != it)
        {
            if ( deleteIt.value().first == master  ||  deleteIt.value().second == slave )
            {
                deleteIt = ratioMasterSlaveMap.erase(deleteIt);
            }
            else
            {
                ++deleteIt;
            }
        }
    }

    return ratioMasterSlaveMap;
}

template<class T> DatabaseItemBase::Ratio DatabaseItem<T>::similarMapToRatio(const QMultiMap<double, QPair<T*, T*>>& map, double threshold)
{
    Ratio r;
    for (double s : map.keys())
    {
        if (s >= threshold) // protect against NaN
        {
            r.d += s;
        }
        else
        {
            r.d += 0.0;
        }
        r.n += 1;
    }
    return r;
}


template class DatabaseItem<Song>;
template class DatabaseItem<Event>;
template class DatabaseItem<SetlistItem>;
template class DatabaseItem<Attachment>;

