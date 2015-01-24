#include "songdatabase.h"
#include "global.h"

SongDatabase::SongDatabase()
{
}

int SongDatabase::columnCount(const QModelIndex &parent) const
{
    assert(!parent.isValid());
    return 1;
}

int SongDatabase::rowCount(const QModelIndex &parent) const
{
    assert(!parent.isValid());
    return m_songs.length();
}

QVariant SongDatabase::data(const QModelIndex &index, int role) const
{
    assert(!index.parent().isValid());

    Q_UNUSED(role);

    if (role == Qt::DisplayRole)
        return m_songs[index.row()]->title();

    return QVariant();
}

QVariant SongDatabase::headerData(int section, Qt::Orientation orientation, int role) const
{
    Q_UNUSED(section);
    Q_UNUSED(orientation);
    Q_UNUSED(role);
    return "title";
}

Qt::ItemFlags SongDatabase::flags(const QModelIndex &index) const
{
    Q_UNUSED(index);
    return Qt::ItemIsEnabled | Qt::ItemIsEditable;
}

bool SongDatabase::setData(const QModelIndex &index, const QVariant &value, int role)
{
    assert(!index.parent().isValid());

    if (role == Qt::EditRole)
    {
        m_songs[index.row()]->setTitle(value.toString());
    }

    return false;
}

bool SongDatabase::insertColumns(int column, int count, const QModelIndex &parent)
{
    assert(!parent.isValid());
    return false;
}

bool SongDatabase::insertRows(int row, int count, const QModelIndex &parent)
{
    assert(!parent.isValid());
    assert( m_tmpSongBuffer.size() == count );

    beginInsertRows(parent, row, row + count - 1);
    for (int i = 0; i < count; ++i)
    {
        m_songs.insert( row + i, m_tmpSongBuffer[i] );
    }
    m_tmpSongBuffer.clear();
    endInsertRows();

    return true;
}

bool SongDatabase::removeColumns(int column, int count, const QModelIndex &parent)
{
    assert(!parent.isValid());
    return false;
}

bool SongDatabase::removeRows(int row, int count, const QModelIndex &parent)
{
    assert(!parent.isValid());
    assert(m_tmpSongBuffer.isEmpty());

    beginRemoveRows(parent, row, row + count - 1);
    for (int i = 0; i < count; ++i)
    {
        m_songs.removeAt(row + i);
    }
    endRemoveRows();

    return true;
}

void SongDatabase::appendSong(Song *song)
{
    m_tmpSongBuffer.append(song);
    assert( insertRows(rowCount(), 1, QModelIndex() ));
}

void SongDatabase::removeSong(Song* song)
{
    int index;
    if ( (index = m_songs.indexOf(song)) < 0 )
    {
        WARNING << "SongDatabase does not contain song " << song;
    }
    else
    {
        assert( removeRows(index, 1, QModelIndex()) );
    }
}


