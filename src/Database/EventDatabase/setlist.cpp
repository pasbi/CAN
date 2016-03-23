#include "setlist.h"

#include "Project/project.h"
#include "event.h"

#include "Database/SongDatabase/song.h"
#include "setlistitem.h"
#include "Attachments/attachment.h"
#include "Database/databasemimedata.h"
#include "Commands/DatabaseCommands/databasenewitemcommand.h"
#include "Commands/DatabaseCommands/databaseeditcommand.h"
#include "Commands/DatabaseCommands/databasemoverowscommand.h"
#include "application.h"


Setlist::Setlist(Event *event) :
    Database(event->database()->project()),
    m_event(event)
{
}

Setlist::~Setlist()
{
}

int Setlist::columnCount( const QModelIndex& parent ) const
{
    Q_UNUSED(parent);
    Q_ASSERT( !parent.isValid());
    return 2;
}

QList<void*> Setlist::viewableAttachments(const QModelIndex &index) const
{
    QList<void*> list;
    SetlistItem* item = itemAtIndex( index );
    const Song* song = item->attribute("song").value<const Song*>();
    if (item && song)
    {
        for (Attachment* attachment : song->attachments())
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
    SetlistItem* item = itemAtIndex(index);
    switch (role)
    {
    case Qt::UserRole:
        return QVariant::fromValue(item);
    case Qt::DisplayRole:
    case Qt::EditRole:
        switch (index.column())
        {
        case 0:
            return item->attributeDisplay("label");
        case 1:
        default:
            return "";
        }
    }
    return QVariant();
}

bool Setlist::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (role == Qt::EditRole)
    {
        SetlistItem* item = itemAtIndex(index);
        if (item)
        {
            item->setAttribute("label", value);
            emit dataChanged(index, index);
            return true;
        }
        else
        {
            return false;
        }
    }
    else
    {
        return false;
    }
}

QList<const Song*> Setlist::songs() const
{
    QList<const Song*> s;
    for (const SetlistItem* item : m_items)
    {
        if (item->attribute("type").value<SetlistItem::Type>() == SetlistItem::SongType)
        {
            s << item->attribute("song").value<Song*>();
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
        return Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsDragEnabled | Qt::ItemIsEditable;
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

bool Setlist::dropSongs(const DatabaseMimeData<Song>* songData, int targetRow, QModelIndexList& indexes)
{
    if (songData->indexedItems().count() > 0)
    {
        app().beginMacro(tr("New Setlist Item"));
        int i = 0;
        for (DatabaseMimeData<Song>::IndexedItem item : songData->indexedItems())
        {
            // create a new setlist item and link `song` with it
            SetlistItem* newItem = new SetlistItem( this, item.item );
            pushCommand( new DatabaseNewItemCommand<SetlistItem>( this, newItem, targetRow + i ) );
            indexes << index(rowOf(newItem), 0);
            i++;
        }
        app().endMacro();
        return true;
    }
    else
    {
        return false;
    }
}

void Setlist::moveItems(const DatabaseMimeData<SetlistItem>* setlistData, int targetRow, QModelIndexList& indexes)
{
    if (setlistData->indexedItems().count() > 0)
    {
        pushCommand( new DatabaseMoveRowsCommand<SetlistItem>(this, setlistData->indexedItemsSorted(), targetRow) );
        for (const DatabaseMimeData<SetlistItem>::IndexedItem& item : setlistData->indexedItems())
        {
            indexes << index(rowOf(item.item), 0);
        }
    }
}

void Setlist::copyItems(const DatabaseMimeData<SetlistItem>* setlistData, int targetRow, QModelIndexList& indexes)
{
    if (setlistData->indexedItems().count() > 0)
    {
        app().beginMacro(tr("Copy Items"));
        int i = 0;
        for (DatabaseMimeData<SetlistItem>::IndexedItem item : setlistData->indexedItems())
        {
            if (true || item.item->database() == this)
            {
                // create a new setlist item and link `song` with it
                SetlistItem* newItem = item.item->copy();
                newItem->setDatabase(this);
                pushCommand( new DatabaseNewItemCommand<SetlistItem>( this, newItem, targetRow + i ));
                indexes << index(rowOf(newItem), 0);
                i++;
            }
            else
            {
                //TODO this is not allowed because copying to other databases is not a good idea in general.
                // however, here it might be usefull and super safe, given that the project is still the same.
                qWarning() << "Cannot drop to different database.";
            }
        }
        app().endMacro();
    }
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

    const DatabaseMimeData<Song>* songData = DatabaseMimeData<Song>::cast(data);
    const DatabaseMimeData<SetlistItem>* setlistData = DatabaseMimeData<SetlistItem>::cast(data);
    QList<QModelIndex> indexes;
    if (songData)
    {
        if (!dropSongs(songData, row, indexes))
        {
            return false;
        }
    }
    else if (setlistData)
    {
        if (action == Qt::MoveAction)
        {
            moveItems(setlistData, row, indexes);
        }
        else if (action == Qt::CopyAction)
        {
            copyItems(setlistData, row, indexes);
        }
        else
        {
            return false;
        }
    }

    QList<int> rows;
    for (const QModelIndex& index : indexes)
    {
        if (!rows.contains(index.row()))
        {
            rows << index.row();
        }
    }
    emit selectionRequest(indexes);
    return true;
}

QStringList Setlist::mimeTypes() const
{
    return QStringList() << DatabaseMimeData<Song>::mimeType() << DatabaseMimeData<SetlistItem>::mimeType();
}

void Setlist::setFilter(QString filter)
{
    m_filter = filter;
    QModelIndex topLeft = index(0, 0);
    QModelIndex bottomRight = index(rowCount() - 1, columnCount() - 1);

    emit dataChanged( topLeft, bottomRight, QVector<int>({Qt::BackgroundRole})  );
}

QString Setlist::itemName(int n) const
{
    return tr("SetlistItem", nullptr, n);
}

void Setlist::setSetlistItems(const QList<SetlistItem*>& items)
{
    beginResetModel();

    qDeleteAll(m_items);
    m_items.clear();

    for (const SetlistItem* item : items)
    {
        m_items << new SetlistItem(this, item->label(), item->attribute("song").value<Song*>());
    }

    endResetModel();
    emit reseted();
}
