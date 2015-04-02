#include "setlist.h"

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
        qDebug() << "return label " << m_label;
        return m_label;
    default:
        return QString();
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
    return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
}

QVariant Setlist::data(const QModelIndex &index, int role) const
{
    assert(index.column() == 0);
    switch (role)
    {
    case Qt::DisplayRole:
        qDebug() << "data: " << m_items[index.row()]->label();
        return m_items[index.row()]->label();
    default:
        return QVariant();
    }
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

