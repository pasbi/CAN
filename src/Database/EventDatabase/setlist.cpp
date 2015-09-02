#include "setlist.h"

#include "application.h"
#include "Project/project.h"
#include "event.h"

#include "Database/SongDatabase/song.h"
#include "setlistitem.h"
#include "Attachments/attachment.h"
#include "Commands/DatabaseCommands/databasenewitemcommand.h"
#include "Commands/DatabaseCommands/databaseedititemcommand.h"
#include "Commands/DatabaseCommands/databasemoverowscommand.h"


Setlist::Setlist(Event *event) :
    Database(event->database()->project()),
    m_event(event)
{
}

int Setlist::columnCount( const QModelIndex& parent ) const
{
    assert( !parent.isValid());
    return 2;
}

QList<void*> Setlist::viewableAttachments(const QModelIndex &index) const
{
    QList<void*> list;
    SetlistItem* item = itemAtIndex( index );
    if (item && item->song())
    {
        for (Attachment* attachment : item->song()->attachments())
        {
            if (attachment->type() == "ChordPatternAttachment")
            {
                list << attachment;
            }
            else if (attachment->type() == "PDFAttachment")
            {
                list << attachment;
            }
        }
    }
    return list;
}

QVariant Setlist::data(const QModelIndex &index, int role) const
{
    switch ( index.column() )
    {
    case 0:
        switch (role)
        {
        case Qt::DisplayRole:
        case Qt::EditRole:
            return m_items[index.row()]->label();
        default:
            return QVariant();
        }
    case 1:
        switch (role)
        {
        case Qt::DisplayRole:
        case Qt::EditRole:
            return ""; //QVariant::fromValue( viewableAttachments( index ) );"
        default:
            return QVariant();
        }

    default:
        return QVariant();
    }

}

bool Setlist::setData(const QModelIndex &index, const QVariant &value, int role)
{
    bool success = true;
    if (role == Qt::EditRole)
    {
        success &= itemAtIndex( index )->setLabel( value.toString() );
        emit dataChanged( index, index );
    }
    else
    {
        success = false;
    }
    return success;
}

//bool Setlist::setData(const QModelIndex &index, const QVariant &value, int role)
//{
//    if (role != Qt::EditRole)
//    {
//        return false;
//    }

//    // push a command only when it is required
//    if (data(index, role) != value)
//    {
//        app().project()->pushCommand( new DatabaseEditItemCommand<SetlistItem>( this, index, value, role ) );
//    }
//    return true;
//}


bool Setlist::moveRows(const QModelIndex &sourceParent, int sourceRow, int count, const QModelIndex &destinationParent, int destinationChild)
{
    if (sourceParent.isValid() || destinationParent.isValid())
    {
        return false;
    }

    if (sourceRow == destinationChild)
    {
        return false;
    }
    if (destinationChild > sourceRow && sourceRow == destinationChild - count)
    {
        destinationChild++;
    }


    beginMoveRows(sourceParent, sourceRow, sourceRow + count - 1, destinationParent, destinationChild);
    QList<SetlistItem*> tmp;
    for (int i = 0; i < count; ++i)
    {
        tmp.prepend( m_items.takeAt(sourceRow) );
    }
    if (destinationChild > sourceRow)
    {
        destinationChild -= count;
    }
    for (int i = 0; i < count; ++i)
    {
        m_items.insert(destinationChild + i, tmp.takeLast());
    }
    endMoveRows();
    return true;
}

QJsonArray Setlist::toJson() const
{
    QJsonArray array;
    for (const SetlistItem* item : m_items)
    {
        array.append( item->toJson() );
    }
    return array;
}

bool Setlist::fromJson(const QJsonArray & array )
{
    beginResetModel();
    bool success = true;
    m_items.clear();
    for (const QJsonValue & val : array)
    {
        SetlistItem* item = new SetlistItem(this);
        item->restoreFromJsonObject(val.toObject());
        if (item)
        {
            m_items << item;
        }
        else
        {
            success = false;
        }
    }
    endResetModel();
    return success;
}

QList<const Song*> Setlist::songs() const
{
    QList<const Song*> s;
    for (const SetlistItem* item : m_items)
    {
        if (item->type() == SetlistItem::SongType)
        {
            s << item->song();
        }
    }
    return s;
}

Qt::ItemFlags Setlist::flags(const QModelIndex &index) const
{
    if (!index.isValid())
    {
        return Qt::ItemIsDropEnabled;
    }
    else
    {
        Qt::ItemFlags flags = Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsDragEnabled;

        if (itemAtIndex(index)->type() == SetlistItem::LabelType && index.column() == 0)
        {
            flags |= Qt::ItemIsEditable;
        }

        return flags;
    }
}

const Event *Setlist::event() const
{
    return m_event;
}

Qt::DropActions Setlist::supportedDragActions() const
{
    return Qt::MoveAction | Qt::CopyAction;
}

Qt::DropActions Setlist::supportedDropActions() const
{
    return Qt::LinkAction | Qt::MoveAction | Qt::CopyAction;
}

#include "Database/databasemimedata.h"
QMimeData* Setlist::mimeData(const QModelIndexList &indexes) const
{
    DatabaseMimeData<SetlistItem>* mime = new DatabaseMimeData<SetlistItem>();

    for (const QModelIndex& index : indexes)
    {
        if (index.column() != 0)
        {
            // we want only one index per row.
            continue;
        }
        mime->append(m_items[index.row()], index.row());
    }
    return mime;
}

bool Setlist::dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent)
{
    if (action == Qt::IgnoreAction)
    {
        return true;
    }

    if (parent.isValid())
    {
        return false;
    }

    Q_UNUSED(column);

    if (row < 0)
    {
        row = rowCount();
    }

    bool success = false;
    const DatabaseMimeData<Song>* songData = DatabaseMimeData<Song>::cast(data);
    const DatabaseMimeData<SetlistItem>* setlistData = DatabaseMimeData<SetlistItem>::cast(data);
    QList<QModelIndex> indexes;
    if (songData)
    {
        if (action == Qt::LinkAction)
        {
            if (songData->indexedItems().count() > 0)
            {
                app().beginMacro(tr("New Setlist Item"));
                int i = 0;
                for (DatabaseMimeData<Song>::IndexedItem item : songData->indexedItems())
                {
                    // create a new setlist item and link `song` with it
                    SetlistItem* newItem = new SetlistItem( this, item.item->copy());
                    app().pushCommand( new DatabaseNewItemCommand<SetlistItem>( this, newItem, row + i ) );
                    indexes << index(rowOf(newItem), 0);
                    i++;
                }
                app().endMacro();
            }
            success = true;
        }
        else
        {
            success = false;
        }
    }
    else if (setlistData)
    {
        if (action == Qt::MoveAction)
        {
            if (setlistData->indexedItems().count() > 0)
            {
                app().pushCommand( new DatabaseMoveRowsCommand<SetlistItem>(this, setlistData->indexedItemsSorted(), row) );
                for (const DatabaseMimeData<SetlistItem>::IndexedItem& item : setlistData->indexedItems())
                {
                    indexes << index(rowOf(item.item), 0);
                }
            }
            success = true;
        }
        else if (action == Qt::CopyAction)
        {
            if (setlistData->indexedItems().count() > 0)
            {
                app().beginMacro(tr("Copy Items"));
                int i = 0;
                for (DatabaseMimeData<SetlistItem>::IndexedItem item : setlistData->indexedItems())
                {
                    // create a new setlist item and link `song` with it
                    SetlistItem* newItem = item.item->copy();
                    app().pushCommand( new DatabaseNewItemCommand<SetlistItem>( this, newItem, row + i ));
                    indexes << index(rowOf(newItem), 0);
                    i++;
                }
                app().endMacro();
            }
            success = true;
        }
        else
        {
            success = false;
        }
    }

    emit selectionRequest(indexes);
    return success;
}


QStringList Setlist::mimeTypes() const
{
    return QStringList() << DatabaseMimeData<Song>::mimeType() << DatabaseMimeData<SetlistItem>::mimeType();
}

void Setlist::saveState()
{
    acceptDrop();
}

void Setlist::restore()
{
    beginResetModel();
    m_items = m_savedState;
    endResetModel();
}

void Setlist::acceptDrop()
{
    m_savedState = m_items;
}




