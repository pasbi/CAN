#include "database.h"
#include "Database/EventDatabase/event.h"
#include "Attachments/attachment.h"

#include "Project/project.h"
#include "Database/databasemimedata.h"
#include "application.h"
#include "Commands/DatabaseCommands/databasenewitemcommand.h"
#include "Database/EventDatabase/setlistitem.h"
#include "SongDatabase/song.h"
#include "util.h"
#include "SongDatabase/attachmentdatabase.h"


template<class T> Database<T>::Database(Project* project) :
    DatabaseBase(project)
{

}


template<class T> Database<T>::~Database()
{
    qDeleteAll(m_items);
    m_items.clear();
}

template<class T> QList<T*> Database<T>::items() const
{
    return m_items;
}

template<class T> T* Database<T>::item(const void* ptr) const
{
    for (T* t : items())
    {
        if (t == ptr)
        {
            return t;
        }
    }
    return nullptr;
}

template<class T> void Database<T>::insertItem(T* item, int row)
{
    if (row < 0)
    {
        row = rowCount();
    }

    beginInsertRows(QModelIndex(), row, row);
    m_items.insert(row, item);
    endInsertRows();
}

template<class T> void Database<T>::removeItem(T* item)
{
    int row = m_items.indexOf(item);
    Q_ASSERT(row >= 0);
    beginRemoveRows(QModelIndex(), row, row);
    m_items.removeAt(row);
    endRemoveRows();
}

template<class T> int Database<T>::rowOf(const T* item) const
{
    return m_items.indexOf(const_cast<T*>(item));
}

template<class T> int Database<T>::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    Q_ASSERT(!parent.isValid());
    return m_items.length();
}

template<class T> void Database<T>::reset()
{
    beginResetModel();
    qDeleteAll( m_items );
    m_items.clear();
    endResetModel();
    DatabaseBase::reset();
}

template<class T> QMimeData* Database<T>::mimeData(const QModelIndexList &indexes) const
{
    DatabaseMimeData<T>* mime = new DatabaseMimeData<T>();
    for (const QModelIndex& index : indexes)
    {
        if (index.column() != 0)
        {
            // we want only one index per row.
            continue;
        }
        mime->append(itemAtIndex(index), index.row());
    }
    return mime;
}

template<class T> bool Database<T>::dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent)
{
    if (action == Qt::IgnoreAction)
    {
        return true;
    }

    if (parent.isValid())
    {
        // no support for drag onto items
        return false;
    }

    // drag'n'drop only whole rows, so column is arbitrary
    Q_UNUSED(column);
    // since
    Q_UNUSED(row);

    // check if data is in right format
    const DatabaseMimeData<T>* itemData = DatabaseMimeData<T>::cast(data);
    if (!itemData)
    {
        return false;
    }

    // copy paste
    if (action == Qt::CopyAction)
    {
        app().beginMacro( DatabaseBase::tr("Paste") );
        int i = 0;
        typedef typename DatabaseMimeData<T>::IndexedItem IndexedItem;
        for (IndexedItem item : itemData->indexedItems())
        {
            Q_ASSERT(item.item->database() == this); // we cannot drop to a different database.
            pushCommand( new DatabaseNewItemCommand<T>( this, item.item->copy(), row + i ) );
            i++;
        }
        app().endMacro();
        return true;
    }
    else
    {
        return false;
    }
    return false;
}

template<class T> T* Database<T>::itemAtIndex(const QModelIndex& index) const
{
    Q_ASSERT( index.model() == this );

    if (index.isValid())
    {
        return m_items[index.row()];
    }
    else
    {
        return nullptr;
    }
}

template<class T> void Database<T>::notifiyDataChange(const T *item)
{
    int row = rowOf(item);
    emit dataChanged( index(row, 0), index(row, columnCount() - 1) );
}

template<class T> T* Database<T>::retrieveItem(qint32 id) const
{
    return m_items[id];
}

template<class T> int Database<T>::identifyItem(const T* item) const
{
    return m_items.indexOf(const_cast<T*>(item));
}

template<class T> QModelIndex Database<T>::indexOf(const T* item, int column)
{
    return index(m_items.indexOf(const_cast<T*>(item)), column);
}

template<class T> QVariant Database<T>::data(const QModelIndex &index, int role) const
{
    Q_ASSERT(!index.parent().isValid());
    const T* item = m_items[index.row()];
    const QString key = item->attributeKeys()[index.column()];
    switch (role)
    {
    case Qt::DisplayRole:
    case Qt::ToolTipRole:
        return item->attributeDisplay(key);
    case Qt::EditRole:
        return item->attribute(key);
    default:
        return QVariant();
    }
}


template<class T> bool Database<T>::setData(const QModelIndex &index, const QVariant &value, int role)
{
    Q_ASSERT(!index.parent().isValid());
    T* item = m_items[index.row()];
    const QString key = item->attributeKeys()[index.column()];
    if (role == Qt::EditRole)
    {
        item->setAttribute(key, value);
        emit dataChanged(index, index);
        return true;
    }
    else
    {
        return false;
    }
}

template<class T> void Database<T>::serialize(QDataStream &out) const
{
    out << static_cast<qint32>(m_items.length());
    for (T* item : m_items)
    {
        out << item;
    }
}

template<class T> void Database<T>::deserialize(QDataStream &in)
{
    reset();
    qint32 n;
    in >> n;
    beginResetModel();
    for (int i = 0; i < n; ++i)
    {
        T* item = new T(this);
        in >> item;
        m_items << item;
    }
    endResetModel();
}

template<> void Database<Attachment>::serialize(QDataStream& out) const
{
    // we need special treatment for Attachments
    out << static_cast<qint32>(items().length());
    for (const Attachment* a : items())
    {
        out << a->type();
        out << a;
    }
}

template<> void Database<Attachment>::deserialize(QDataStream& in)
{
    Song* song = static_cast<const AttachmentDatabase*>(this)->song();
    // we need special treatment for Attachments
    qint32 n;
    in >> n;
    reset();
    beginResetModel();
    for (int i = 0; i < n; ++i)
    {
        QString typeName;
        in >> typeName;

        Attachment* attachment = Attachment::create(typeName);
        attachment->setSong(song);
        in >> attachment;
        m_items << attachment;
    }
    endResetModel();
}


// instanciations
template class Database<Song>;
template class Database<Event>;
template class Database<SetlistItem>;
template class Database<Attachment>;
