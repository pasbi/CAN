#include "setlist.h"
#include "application.h"
#include "project.h"

QString SetlistItem::label() const
{
    switch (m_type)
    {
    case SongType:
    {
        QString title = m_song->title();
        if (title.isEmpty())
        {
            return QString("%1").arg(m_song->title());
        }
        else
        {
            return QString("%1 - %2").arg(m_song->title(), m_song->artist());
        }
    }
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
        qWarning() << "Cannot set label of non-label type SetlistItem.";
        return false;
    }
}

Setlist::Setlist(QObject *parent) :
    QAbstractListModel(parent)
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

int Setlist::rowCount(const QModelIndex &parent) const
{
    assert( !parent.isValid() );
    return m_items.length();
}

Qt::ItemFlags Setlist::flags(const QModelIndex &index) const
{
    Q_UNUSED( index );
    switch (itemAt(index)->type())
    {
    case SetlistItem::LabelType:
        return Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsEditable;
    case SetlistItem::SongType:
        return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
    default:
        return Qt::ItemFlags();
    }

}

QVariant Setlist::data(const QModelIndex &index, int role) const
{
    assert(index.column() == 0);
    switch (role)
    {
    case Qt::DisplayRole:
    case Qt::EditRole:
        return m_items[index.row()]->label();
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
    app().project()->pushCommand( new SetlistEditDataCommand( this, index, value, role ) );
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
