#include "songdatabase.h"
#include "global.h"
#include "project.h"


SongDatabase::SongDatabase(Project *project) :
    QAbstractTableModel( 0 ),
    Database(project)
{
    m_attributeKeys << "Hello" << "Wrodl";
}

int SongDatabase::columnCount(const QModelIndex &parent) const
{
    assert(!parent.isValid());
    return m_attributeKeys.size();
}

int SongDatabase::rowCount(const QModelIndex &parent) const
{
    assert(!parent.isValid());
    return m_songs.length();
}

void editorTypeAndHeaderLabel( const QString & encoding, QString & editorType, QString & attributeValue )
{
    // "a:b" -> "a", "b"
    // "a" -> "", "a"
    // "a:b:c" -> "a", "b:c"

    QStringList token = encoding.split(":");
    if (token.size() == 1)
    {
        editorType = "";
        attributeValue = token.first();
    }
    else
    {
        editorType = token.takeFirst();
        attributeValue = token.join(":");
    }
}

QString extractEditorType(const QString & encoding)
{
    QString ed, lb;
    editorTypeAndHeaderLabel( encoding, ed, lb );
    return ed;
}

QString extractHeaderLabel(const QString & encoding)
{
    QString ed, lb;
    editorTypeAndHeaderLabel( encoding, ed, lb );
    return lb;
}

QVariant SongDatabase::data(const QModelIndex &index, int role) const
{
    assert(!index.parent().isValid());


    switch (role)
    {
    case Qt::DisplayRole:
        return m_songs[index.row()]->attribute(index.column());
    case Qt::EditRole:
        return m_songs[index.row()]->attribute(index.column());
    default:
        return QVariant();
    }
}

QVariant SongDatabase::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal)
    {
        switch (role)
        {
        case Qt::DisplayRole:
            return extractHeaderLabel(m_attributeKeys[section]);
        case Qt::EditRole:
            return m_attributeKeys[section];
        default:
            return QVariant();
        }
    }
    else
    {
        return QVariant();
    }
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
        m_songs[index.row()]->setAttribute(index.column(), value);
        emit dataChanged( index, index );
    }

    return false;
}

void SongDatabase::notifyDataChanged(const QModelIndex &index)
{
    notifyDataChanged(index, index);
}

void SongDatabase::notifyDataChanged(const QModelIndex & start, const QModelIndex & end)
{
    emit dataChanged(start, end);
}

void SongDatabase::notifyDataChanged( Song* song )
{
    int row = m_songs.indexOf(song);
    if (row < 0)
    {
        return;
    }

    QModelIndex start = index( row, 0, QModelIndex() );
    QModelIndex end   = index( row, columnCount() - 1, QModelIndex() );

    notifyDataChanged( start, end );
}

bool SongDatabase::insertColumns(int column, int count, const QModelIndex &parent)
{
    assert( !parent.isValid() );
    assert( m_tmpColumnNameBuffer.size() == count );

    beginInsertColumns(parent, column, column + count - 1);
    for (int i = 0; i < count; ++i)
    {
        m_attributeKeys.insert( column + i,  m_tmpColumnNameBuffer[i]);
        for (Song* song : m_songs)
        {
            song->insertAttribute(column + i, QVariant());
        }
    }
    endInsertColumns();
    m_tmpColumnNameBuffer.clear();

    return true;
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
    beginRemoveColumns(parent, column, column + count - 1);
    for (int i = 0; i < count; ++i)
    {
        m_attributeKeys.removeAt(column + i);
        for (Song* s : songs())
        {
            s->removeAttribute(column + i);
        }
    }
    endRemoveColumns();
    return true;
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
    insertSong(song, rowCount());
}

void SongDatabase::insertSong(Song* song, const int index)
{    
    m_tmpSongBuffer.append(song);
    assert( insertRows( index, 1, QModelIndex() ));
}

int SongDatabase::removeSong(Song* song)
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
    return index;
}

Song* SongDatabase::songAtIndex(const QModelIndex &index) const
{
    if (index.isValid() && index.row() < rowCount())
    {
        return songs().at(index.row());
    }
    else
    {
        return NULL;
    }
}

void SongDatabase::insertColumn(const int section, const QString &label)
{
    m_tmpColumnNameBuffer.append(label);
    assert( insertColumns(section, 1, QModelIndex()) );
}

void SongDatabase::appendColumn(const QString &label)
{
    insertColumn( columnCount(), label );
}

bool SongDatabase::setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role)
{
    if (orientation == Qt::Horizontal && role == Qt::EditRole && section >= 0 && section < columnCount())
    {
        m_attributeKeys[section] = value.toString();
        emit headerDataChanged(orientation, section, section);
        return true;
    }
    else
    {
        return false;
    }
}

QString SongDatabase::editorType( const QModelIndex & index ) const
{
    return extractEditorType( m_attributeKeys[index.column()] );
}

QVariant SongDatabase::data(const int row, const int column, const int role)
{
    return data(index(row, column, QModelIndex()), role);
}


