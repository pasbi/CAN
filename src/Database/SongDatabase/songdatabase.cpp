#include "songdatabase.h"
#include "Project/project.h"
#include "Commands/SongDatabaseCommands/songdatabasenewattributecommand.h"
#include "songdatabasesortproxy.h"
#include <QSize>
#include "application.h"
#include "commontypes.h"
#include <QJsonDocument>

SongDatabase::SongDatabase(Project *project) :
    Database(project),
    m_columnIsVisible(columnCount(), true)  // all columns are visible by default.
{
    Song::seedRandomID();
}

int SongDatabase::columnCount(const QModelIndex &parent) const
{
    assert(!parent.isValid());
    return 4;
}

int SongDatabase::rowCount(const QModelIndex &parent) const
{
    assert(!parent.isValid());
    return m_items.length();
}

QVariant SongDatabase::data(const QModelIndex &index, int role) const
{
    assert(!index.parent().isValid());

    switch (role)
    {
    case Qt::DisplayRole:
    case Qt::EditRole:
    {
        const Song* song = m_items[index.row()];
        switch (index.column())
        {
        case 0:
            return song->title();
        case 1:
            return song->artist();
        case 2:
            return song->creationTime();
        case 3:
            return song->duration();
        }
    }
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
        case Qt::EditRole:
            switch (section)
            {
            case 0:
                return tr("Title");
            case 1:
                return tr("Artist");
            case 2:
                return tr("Creation Date");
            case 3:
                return tr("Duration");
            }

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
    Qt::ItemFlags flags = Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsDragEnabled;
    if (index.column() != 2)
    {
        flags |= Qt::ItemIsEditable;
    }
    return flags;
}

bool SongDatabase::setData(const QModelIndex &index, const QVariant &value, int role)
{
    assert(!index.parent().isValid());

    if (role == Qt::EditRole)
    {
        Song* song = m_items[index.row()];
        switch (index.column())
        {
        case 0:
            song->setTitle(value.toString());
            break;
        case 1:
            song->setArtist(value.toString());
            break;
        case 2:
            qWarning() << "Cannot set read-only value CreationDateTime";
        case 3:
            song->setDuration(value.toTime());
            break;
        }
        return true;
    }

    return false;
}

bool SongDatabase::insertRows(int row, int count, const QModelIndex &parent)
{
    assert(!parent.isValid());
    assert( m_tmpSongBuffer.size() == count );

    beginInsertRows(parent, row, row + count - 1);
    for (int i = 0; i < count; ++i)
    {
        m_items.insert( row + i, m_tmpSongBuffer[i] );
        emit songAdded( row + i, m_tmpSongBuffer[i] );
    }
    m_tmpSongBuffer.clear();
    endInsertRows();

    return true;
}

bool SongDatabase::removeRows(int row, int count, const QModelIndex &parent)
{
    assert(!parent.isValid());
    assert(m_tmpSongBuffer.isEmpty());

    beginRemoveRows(parent, row, row + count - 1);
    for (int i = 0; i < count; ++i)
    {
        m_items.removeAt(row + i);
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
    if ( (index = m_items.indexOf(song)) < 0 )
    {
        WARNING << "SongDatabase does not contain song " << song;
    }
    else
    {
        assert( removeRows(index, 1, QModelIndex()) );
    }
    return index;
}

QVariant SongDatabase::data(const int row, const int column, const int role)
{
    return data(index(row, column, QModelIndex()), role);
}

QJsonObject SongDatabase::toJsonObject() const
{
    QJsonObject json;
    json["id"] = randomID();
    return json;
}

bool SongDatabase::restoreFromJsonObject(const QJsonObject &object)
{
    m_randomID = object["id"].toString();
    return true;
}

QList<File> SongDatabase::getFiles() const
{
    QList<File> files;
    files << File( "songDatabase", QJsonDocument(toJsonObject()).toJson() );

    for (int i = 0; i < m_items.size(); ++i)
    {
        files << File( QString("song%1").arg( m_items[i]->randomID() ),
                       QJsonDocument(m_items[i]->toJsonObject()).toJson() );
    }

    return files;
}

bool SongDatabase::loadFrom(const QString &path)
{
    bool success = true;


    beginResetModel();
    m_items.clear();
    if (Database::loadFrom(path))
    {
        blockSignals(true);
        QStringList filenames = QDir( project()->path() ).entryList( QStringList() << "song*" );
        filenames.removeOne( "songDatabase" );

        for (const QString& filename : filenames)
        {
            QString path = project()->makeAbsolute( filename );
            Song* s = new Song(this);
            s->loadFrom(path);
            appendSong( s );
        }
        blockSignals(false);

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

void SongDatabase::reset()
{
    beginResetModel();
    m_items.clear();
    endResetModel();

    emit attachmentAdded( -1 );
}


Song* SongDatabase::song( const QString& id ) const
{
    for (Song* song : m_items)
    {
        if (song->randomID() == id)
        {
            return song;
        }
    }
    return NULL;
}

QModelIndex SongDatabase::indexOfSong(const Song *song) const
{
    int row = -1;
    for (int i = 0; i < m_items.length() && row < 0; ++i)
    {
        if (song == m_items[i])
        {
            row = i;
        }
    }
    assert( row >= 0 );

    return createIndex( row, 0 );
}

#include "Database/databasemimedata.h"
QMimeData* SongDatabase::mimeData(const QModelIndexList &indexes) const
{
    DatabaseMimeData<Song>* mime = new DatabaseMimeData<Song>();
    for (const QModelIndex& index : indexes)
    {
        qDebug() << "request mimedata from row " << index.row();
        if (index.column() != 0)
        {
            // we want only one index per row.
            continue;
        }
        mime->append(itemAtIndex(index), index.row());
    }
    return mime;
}

#include "Commands/SongDatabaseCommands/songdatabasenewsongcommand.h"
bool SongDatabase::dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent)
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
    const DatabaseMimeData<Song>* songData = DatabaseMimeData<Song>::cast(data);
    if (!songData)
    {
        return false;
    }

    // copy paste
    if (action == Qt::CopyAction)
    {
        app().beginMacro( tr("Copy songs") );
        int i = 0;
        for (DatabaseMimeData<Song>::IndexedItem item : songData->indexedItems())
        {
            app().pushCommand( new SongDatabaseNewSongCommand( this, item.item->copy(), row + i ) );
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

Qt::DropActions SongDatabase::supportedDragActions() const
{
    return Qt::LinkAction;
}

Qt::DropActions SongDatabase::supportedDropActions() const
{
    return Qt::IgnoreAction;
}
