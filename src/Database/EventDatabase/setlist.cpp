#include "setlist.h"
#include "application.h"
#include "project.h"

QString SetlistItem::label() const
{
    switch (m_type)
    {
    case SongType:
        return QString("%1 - %2").arg(m_song->title(), m_song->artist());
    case LabelType:
        return m_label;
    default:
        return QString();
    }
}

bool SetlistItem::setLabel(const QString label)
{
    if (type() == LabelType)
    {
        m_label = label;
        return true;
    }
    else
    {
        // ignore.
        return false;
    }
}

QJsonObject SetlistItem::toJson() const
{
    QJsonObject object;

    object["type"] = (int) m_type;
    object["SongID"] = app().project()->songDatabase()->songID( m_song );
    object["Label"] = m_label;

    return object;
}

SetlistItem* SetlistItem::fromJson( const QJsonObject & object )
{
    if (!checkJsonObject( object, "type", QJsonValue::Double ))
    {
        return NULL;
    }
    switch ((Type) object["type"].toInt())
    {
    case SongType:
        if (!checkJsonObject(object, "SongID", QJsonValue::Double))
        {
            return NULL;
        }
        else
        {
            Song* song = app().project()->songDatabase()->song( object["SongID"].toInt() );
            if (song)
            {
                return new SetlistItem( song );
            }
            else
            {
                return NULL;
            }
        }
    case LabelType:
        if (!checkJsonObject(object, "Label", QJsonValue::String))
        {
            return NULL;
        }
        else
        {
            return new SetlistItem( object["Label"].toString() );
        }
    }
    return NULL;
}

QDataStream& operator << (QDataStream& out, const SetlistItem* item )
{
    out << (quint16) item->m_type;
    switch (item->m_type)
    {
    case SetlistItem::SongType:
        out << (qintptr) item->m_song;
        break;
    case SetlistItem::LabelType:
        out << item->m_label;
        break;
    }
    return out;
}

QDataStream& operator >> (QDataStream& in,  SetlistItem* &item )
{
    quint16 type;
    in >> type;
    switch ((SetlistItem::Type) type)
    {
    case SetlistItem::SongType:
    {
        qintptr ptr;
        in >> ptr;
        item = new SetlistItem( (Song*) ptr );
        break;
    }
    case SetlistItem::LabelType:
    {
        QString string;
        in >> string;
        item = new SetlistItem( string );
        break;
    }
    default:
        item = NULL;
    }
    return in;
}

Setlist::Setlist(Event *event) :
    QAbstractTableModel(event),
    m_event( event )
{

}

void Setlist::insertItem( int position, SetlistItem* item )
{
    assert(m_tmpItemBuffer.isEmpty());
    m_tmpItemBuffer << item;
    insertRow( position );
}

void Setlist::removeItem( SetlistItem* item )
{
    int pos = m_items.indexOf( item );
    if (pos >= 0)
    {
        removeRow( pos );
    }
}

int Setlist::rowCount(const QModelIndex& parent) const
{
    assert( !parent.isValid() );
    return m_items.length();
}

Qt::ItemFlags Setlist::flags(const QModelIndex &index) const
{
    Q_UNUSED( index );
    switch (index.column())
    {
    case 0:
        switch (itemAt(index)->type())
        {
        case SetlistItem::LabelType:
            return Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsDragEnabled | Qt::ItemIsEditable;
        case SetlistItem::SongType:
            return Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsDragEnabled;
        default:
            return Qt::ItemFlags();
        }
    case 1:
        switch (itemAt(index)->type())
        {
        case SetlistItem::LabelType:
            return Qt::ItemIsSelectable | Qt::ItemIsDragEnabled;
        case SetlistItem::SongType:
            return Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsDragEnabled | Qt::ItemIsEditable;
        default:
            return Qt::ItemFlags();
        }
    }
    return Qt::ItemFlags();
}

QList<void*> Setlist::viewableAttachments(const QModelIndex &index) const
{
    QList<void*> list;
    SetlistItem* item = itemAt( index );
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

bool Setlist::setData_(const QModelIndex &index, const QVariant &value, int role)
{
    bool success = true;
    if (role == Qt::EditRole)
    {
        success &= itemAt( index )->setLabel( value.toString() );
        emit dataChanged( index, index );
    }
    else
    {
        success = false;
    }
    return success;
}

#include "Commands/SetlistCommands/setlisteditdatacommand.h"
bool Setlist::setData(const QModelIndex &index, const QVariant &value, int role)
{
    // push a command only when it is required
    if (data(index, role) != value)
    {
        app().project()->pushCommand( new SetlistEditDataCommand( this, index, value, role ) );
    }
    return true;
}

bool Setlist::insertRows(int row, int count, const QModelIndex &parent)
{
    assert(count == m_tmpItemBuffer.length());
    beginInsertRows(parent, row, row + count - 1);
    for (int i = 0; i < count; ++i)
    {
        m_items.insert(row + i, m_tmpItemBuffer[i]);
    }
    m_tmpItemBuffer.clear();
    endInsertRows();
    return true;
}

bool Setlist::removeRows(int row, int count, const QModelIndex &parent)
{
    beginRemoveRows(parent, row, row + count - 1);
    for (int i = row + count - 1; i >= row; --i)
    {
        m_items.removeAt(i);
    }
    endRemoveRows();
    return true;
}

void Setlist::appendItem(SetlistItem *item)
{
    insertItem( m_items.length(), item );
}

int Setlist::indexOf(const SetlistItem *item) const
{
    // QList::indexOf forbids item to be const

    int index = 0;
    for (const SetlistItem* i : m_items)
    {
        if (i == item)
        {
            return index;
        }
        index++;
    }
    return -1;
}

SetlistItem* Setlist::itemAt(const QModelIndex &index) const
{
    return m_items[index.row()];
}

void Setlist::notifyDataChanged(const QModelIndex &index)
{
    notifyDataChanged( index, index );
}

void Setlist::notifyDataChanged(const QModelIndex & start, const QModelIndex & end)
{
    emit dataChanged( start, end );
}

void Setlist::notifyDataChanged(const SetlistItem *item)
{
    int i = indexOf(item);
    notifyDataChanged( index(i, 0) );
}

Qt::DropActions Setlist::supportedDragActions() const
{
    return Qt::MoveAction | Qt::CopyAction;
}

QMimeData* Setlist::mimeData(const QModelIndexList &indexes) const
{
    QMimeData* mime = new QMimeData();

    QByteArray data;
    QDataStream stream(&data, QIODevice::WriteOnly);

    m_draggedItems.clear();
    for (const QModelIndex& index : indexes)
    {
        if (index.column() != 0)
        {
            // we want only one index per row.
            continue;
        }
        m_draggedItems << m_items[index.row()];
    }
    stream << m_draggedItems;

    mime->setData("CAN/Setlist/Item", data);
    return mime;
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
        SetlistItem* item = SetlistItem::fromJson(val.toObject());
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

#include "Commands/SetlistCommands/setlistremoveitemcommand.h"
void Setlist::removeDraggedItems()
{
    app().project()->beginMacro(tr("Remove dragged items"));
    for ( SetlistItem* item : m_draggedItems)
    {
        app().project()->pushCommand( new SetlistRemoveItemCommand( this, item ) );
    }
    m_draggedItems.clear();
    app().project()->endMacro();
}

void Setlist::moveItem(int from, int to)
{
    if (from > to)
    {
        beginMoveRows( QModelIndex(), from, from, QModelIndex(), to );
        m_items.insert( to, m_items.takeAt( from ) );
        endMoveRows();
    }
    else
    {
        beginMoveRows( QModelIndex(), from, from, QModelIndex(), to + 1 );
        m_items.insert( to, m_items.takeAt( from ) );
        endMoveRows();
    }
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

const Event *Setlist::event() const
{
    return m_event;
}

