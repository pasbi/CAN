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




template class DatabaseItem<Song>;
template class DatabaseItem<Event>;
template class DatabaseItem<SetlistItem>;
template class DatabaseItem<Attachment>;

