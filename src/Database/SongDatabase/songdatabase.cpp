#include "songdatabase.h"
#include "project.h"
#include "Commands/SongDatabaseCommands/songdatabaseremovesongcommand.h"
#include "Commands/SongDatabaseCommands/songdatabasenewsongcommand.h"
#include "Commands/SongDatabaseCommands/songdatabaseremovecolumncommand.h"
#include "Commands/SongDatabaseCommands/songdatabasenewattributecommand.h"
#include "songdatabasesortproxy.h"
#include <QSize>
#include "application.h"
#include "commontypes.h"


SongDatabase::SongDatabase(Project *project) :
    QAbstractTableModel( 0 ),
    Database(project)
{
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
    // "a:b"        -> "a", "b"
    // "a"          -> "", "a"
    // "a:b:c"      -> "a", "b:c"

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

QString SongDatabase::extractEditorType(const QString & key)
{
    QString ed, lb;
    editorTypeAndHeaderLabel( key, ed, lb );
    return ed;
}

QString SongDatabase::extractHeaderLabel(const QString & key)
{
    QString ed, lb;
    editorTypeAndHeaderLabel( key, ed, lb );
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
    if (orientation == Qt::Vertical)
    {
        switch (role)
        {
        case Qt::SizeHintRole:
        {
            return QSize(10, 10);
        }
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
    return Qt::ItemIsEnabled | Qt::ItemIsEditable | Qt::ItemIsSelectable | Qt::ItemIsDragEnabled;
}

bool SongDatabase::setData(const QModelIndex &index, const QVariant &value, int role)
{
    assert(!index.parent().isValid());

    if (role == Qt::EditRole)
    {
        m_songs[index.row()]->setAttribute(index.column(), value);
        emit dataChanged( index, index );
        return true;
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

void SongDatabase::notifyDataChanged( const Song* song )
{
    int row = m_songs.indexOf((Song*) song);
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
        emit songAdded( row + i, m_tmpSongBuffer[i] );
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
        emit songRemoved(row + i );
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
    connect( song, SIGNAL(attachmentAdded(int)),   this, SIGNAL(attachmentAdded(int)  ));
    connect( song, SIGNAL(attachmentRemoved(int)), this, SIGNAL(attachmentRemoved(int)));
    connect( song, SIGNAL(attachmentRenamed(int, QString)), this, SIGNAL(attachmentRenamed(int,QString)));
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

QJsonObject SongDatabase::toJsonObject() const
{
    QJsonObject meta;
    meta.insert("numsongs", m_songs.size());

    QJsonArray attributeKeys;
    for (int i = 0; i < m_attributeKeys.size(); ++i)
    {
        attributeKeys.append(m_attributeKeys[i]);
    }
    meta.insert("attributekeys", attributeKeys);

    return meta;
}

bool SongDatabase::restoreFromJsonObject(const QJsonObject &object)
{

    if (       !checkJsonObject(object, "numsongs", QJsonValue::Double)
            || !checkJsonObject(object, "attributekeys", QJsonValue::Array)  )
    {
        return false;
    }

    checkJsonObject(object, "numsongs", QJsonValue::Double);

    m_numSongsToRestore = object["numsongs"].toInt();
    QJsonArray attributekeys = object["attributekeys"].toArray();

    m_attributeKeysToRestore.clear();
    for (const QJsonValue & val : attributekeys )
    {
        m_attributeKeysToRestore.append(val.toString());
    }

    return m_numSongsToRestore >= 0;
}

bool SongDatabase::saveTo(const QString &path) const
{
    Database::saveTo(path);
    for (int i = 0; i < m_songs.size(); ++i)
    {
        QString path = project()->makeAbsolute( QString("song%1").arg(i) );
        m_songs[i]->saveTo(path);
    }

    return true;
}

bool SongDatabase::loadFrom(const QString &path)
{
    bool success = true;

    reset(false);

    beginResetModel();

    if (Database::loadFrom(path))
    {

        for (int i = 0; i < m_attributeKeysToRestore.size(); ++i)
        {
            app().pushCommand( new SongDatabaseNewAttributeCommand( this, m_attributeKeysToRestore[i] ));
        }

        for (int i = 0; i < m_numSongsToRestore; ++i)
        {
            QString path = project()->makeAbsolute( QString("song%1").arg(i) );
            Song* s = new Song(this);
            s->loadFrom(path);
            appendSong( s );
        }

    }
    else
    {
        WARNING << "Loading failed.";
        success = false;
    }

    endResetModel();

    emit attachmentAdded( -1 );
    return success;
}

void SongDatabase::initAttributes()
{
    appendColumn(tr("Song"));
    appendColumn(tr("Combo:Artist"));
}

void SongDatabase::reset(bool initialize)
{
    project()->QUndoStack::clear();
    beginResetModel();
    m_songs.clear();
    m_attributeKeys.clear();
    endResetModel();

    if (initialize)
    {
        initAttributes();
    }
    emit attachmentAdded( -1 );
}

SongID SongDatabase::songID( const Song* song ) const
{
    for (int i = 0; i < m_songs.length(); ++i)
    {
        if (song == m_songs[i])
        {
            return i;
        }
    }
    return -1;
}

Song* SongDatabase::song( SongID id ) const
{
    if (id < 0 || id >= m_songs.length())
    {
        qWarning() << "song with id " << id << " does not exist";
        return NULL;
    }
    else
    {
        return m_songs[id];
    }
}

Qt::DropActions SongDatabase::supportedDragActions() const
{
    return Qt::CopyAction;
}

QMimeData* SongDatabase::mimeData(const QModelIndexList &indexes) const
{
    QMimeData* mime = new QMimeData();

    QByteArray data;
    QDataStream stream(&data, QIODevice::WriteOnly);

    QList<qintptr> ptrs;
    for (const QModelIndex& index : indexes)
    {
        if (index.column() != 0)
        {
            // we want only one index per row.
            continue;
        }
        Song* song = songAtIndex(index);
        ptrs << qintptr(song);
    }
    stream << ptrs;

    mime->setData("CAN/songs", data);
    return mime;
}

