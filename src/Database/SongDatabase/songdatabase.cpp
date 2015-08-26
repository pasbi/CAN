#include "songdatabase.h"
#include "Project/project.h"
#include "Commands/SongDatabaseCommands/songdatabasenewattributecommand.h"
#include "songdatabasesortproxy.h"
#include <QSize>
#include "application.h"
#include "commontypes.h"
#include <QJsonDocument>

QList<SongDatabase::InitialColumn> initColumns()
{
#define COLUMN( CAPTION, FIX_EDITOR ) SongDatabase::InitialColumn({ SongDatabase::tr(CAPTION), FIX_EDITOR })
    QList<SongDatabase::InitialColumn> cs;
    cs << COLUMN( "Title", false )
       << COLUMN( "Combo:Artist", false )
       << COLUMN( "Date:Date Added", true )
       << COLUMN( "Duration:Duration", true );

    return cs;
#undef COLUMN
}

const QList<SongDatabase::InitialColumn> SongDatabase::INITIAL_COLUMNS = initColumns();

SongDatabase::SongDatabase(Project *project) :
    Database(project)
{
    Song::seedRandomID();
}

void SongDatabase::initAttributes()
{
    for (const InitialColumn& ic : INITIAL_COLUMNS)
    {
        appendColumn( ic.caption );
    }
}

bool SongDatabase::allowDeleteColumn(int i)
{
    if (i >= INITIAL_COLUMNS.length())
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool SongDatabase::fixColumnEditor(int i)
{
    if (i >= INITIAL_COLUMNS.length())
    {
        return true;
    }
    else
    {
        return INITIAL_COLUMNS[i].fixEditor;
    }
}

int SongDatabase::columnCount(const QModelIndex &parent) const
{
    assert(!parent.isValid());
    return m_attributeKeys.size();
}

int SongDatabase::rowCount(const QModelIndex &parent) const
{
    assert(!parent.isValid());
    return m_items.length();
}

void SongDatabase::editorTypeAndHeaderLabel( const QString & encoding, QString & editorType, QString & attributeValue )
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
    case Qt::EditRole:
        return m_items[index.row()]->attribute(index.column());
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
        m_items[index.row()]->setAttribute(index.column(), value);
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
    int row = m_items.indexOf((Song*) song);
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
    assert( m_tmpColumnContentBuffer.size() == count || m_tmpColumnContentBuffer.isEmpty() );

    beginInsertColumns(parent, column, column + count - 1);
    for (int i = 0; i < count; ++i)
    {
        m_attributeKeys.insert( column + i,  m_tmpColumnNameBuffer[i]);
        for (int j = 0; j < m_items.length(); ++j)
        {
            QVariant attribute;
            if (!m_tmpColumnContentBuffer.isEmpty())
            {
                attribute = m_tmpColumnContentBuffer[i][j];
            }
            m_items[j]->insertAttribute(column + i, attribute);
        }
    }
    endInsertColumns();
    m_tmpColumnNameBuffer.clear();
    m_tmpColumnContentBuffer.clear();

    return true;
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

bool SongDatabase::removeColumns(int column, int count, const QModelIndex &parent)
{
    assert(!parent.isValid());
    beginRemoveColumns(parent, column, column + count - 1);
    for (int i = 0; i < count; ++i)
    {
        m_attributeKeys.removeAt(column + i);
        for (Song* s : items())
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

void SongDatabase::insertColumn(const int section, const AttributeKey &label)
{
    m_tmpColumnNameBuffer.append(label);
    assert( insertColumns(section, 1, QModelIndex()) );
}

void SongDatabase::appendColumn(const AttributeKey &label)
{
    insertColumn( columnCount(), label );
}

void SongDatabase::restoreColumn(const int section, const QString &label, const QVariantList &attributes)
{
    beginResetModel();  // required for neat displaying
    assert( m_tmpColumnContentBuffer.isEmpty() );
    m_tmpColumnContentBuffer.append( attributes );
    insertColumn( section, label );
    endResetModel();
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
    QJsonObject json;
    json["id"] = randomID();

    QJsonArray attributeKeys;
    for (int i = 0; i < m_attributeKeys.size(); ++i)
    {
        attributeKeys.append(m_attributeKeys[i].toJson());
    }
    json.insert("attributekeys", attributeKeys);

    return json;
}

bool SongDatabase::restoreFromJsonObject(const QJsonObject &object)
{

    if ( !checkJsonObject(object, "attributekeys", QJsonValue::Array)  )
    {
        return false;
    }

    QJsonArray attributekeys = object["attributekeys"].toArray();
    m_randomID = object["id"].toString();

    m_attributeKeysToRestore.clear();
    for (const QJsonValue & val : attributekeys )
    {
        m_attributeKeysToRestore.append(AttributeKey::fromJson(val.toObject()));
    }

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

    reset(false);

    beginResetModel();

    if (Database::loadFrom(path))
    {
        blockSignals(true);
        for (int i = 0; i < m_attributeKeysToRestore.size(); ++i)
        {
            app().pushCommand( new SongDatabaseNewAttributeCommand( this, m_attributeKeysToRestore[i] ));
        }

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

void SongDatabase::reset(bool initialize)
{
    beginResetModel();
    m_items.clear();
    m_attributeKeys.clear();
    endResetModel();

    if (initialize)
    {
        initAttributes();
    }
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

void SongDatabase::moveRow(int sourceRow, int targetRow)
{
    assert( moveRows( QModelIndex(), sourceRow, 1, QModelIndex(), targetRow ));
}

bool SongDatabase::moveRows(const QModelIndex &sourceParent, int sourceRow, int count, const QModelIndex &destinationParent, int destinationChild)
{
    assert( !sourceParent.isValid() );
    assert( !destinationParent.isValid() );

    destinationChild = qBound( 0, destinationChild, m_items.length() - count );

    int diff = destinationChild - sourceRow;
    if (diff > 0)
    {
        assert( beginMoveRows( sourceParent, sourceRow, sourceRow + count - 1, destinationParent, destinationChild + 1 ) );
        for (int i = sourceRow + count - 1; i >= sourceRow; --i)
        {
            m_items.insert(i + diff, m_items.takeAt(i));
        }
        endMoveRows();
        return true;
    }
    else if (diff < 0)
    {
        assert( beginMoveRows( sourceParent, sourceRow, sourceRow + count - 1, destinationParent, destinationChild ) );
        for (int i = sourceRow; i <= sourceRow + count - 1; ++i)
        {
            m_items.insert(i + diff, m_items.takeAt(i));
        }
        endMoveRows();
        return true;
    }
    else
    {
        return false;
    }
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

QStringList SongDatabase::attributeKeys() const
{
    QStringList keys;
    for (const AttributeKey & key : m_attributeKeys)
    {
        keys << key;
    }
    return keys;
}

bool SongDatabase::attributeVisible(int i)
{
    return m_attributeKeys[i].visible;
}

void SongDatabase::setAttributeVisible(int i, bool visible)
{
    beginResetModel();
    m_attributeKeys[i].visible = visible;
    endResetModel();
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
