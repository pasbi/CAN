#include "eventdatabase.h"
#include "Project/project.h"
#include "Commands/EventDatabaseCommands/eventdatabaseediteventcommand.h"
#include "application.h"
#include <QJsonDocument>
#include "Database/databasemimedata.h"

EventDatabase::EventDatabase(Project *project) :
    Database<Event>(project)
{
    Event::seedRandomID();
}

QList<File> EventDatabase::getFiles() const
{
    QList<File> files;
    files << File( "eventDatabase", QJsonDocument(toJsonObject()).toJson() );

    for (int i = 0; i < m_items.size(); ++i)
    {
        files << File( QString("event%1").arg( m_items[i]->randomID() ),
                       QJsonDocument(m_items[i]->toJsonObject()).toJson() );
    }

    return files;
}

QJsonObject EventDatabase::toJsonObject() const
{
    return QJsonObject();
}

Qt::DropActions EventDatabase::supportedDragActions() const
{
    return Qt::IgnoreAction;
}

Qt::DropActions EventDatabase::supportedDropActions() const
{
    return Qt::IgnoreAction;
}

bool EventDatabase::restoreFromJsonObject(const QJsonObject &object)
{
    Q_UNUSED( object );

    beginResetModel();
    bool success = true;

    m_items.clear();
    QStringList filenames = QDir( project()->path() ).entryList( QStringList() << "event*" );
    filenames.removeOne( "eventDatabase" );

    for (const QString& filename : filenames)
    {
        Event* e = new Event( this );
        success &= e->loadFrom( project()->makeAbsolute( filename ) );
        m_items << e;
    }

    endResetModel();
    return success;
}

void EventDatabase::reset()
{
    beginResetModel();
    qDeleteAll( m_items );
    m_items.clear();
    endResetModel();
}

int EventDatabase::rowCount(const QModelIndex &parent) const
{
    assert( !parent.isValid() );
    return m_items.length();
}

int EventDatabase::columnCount(const QModelIndex &parent) const
{
    //    Type | Beginning | Label
    assert( !parent.isValid() );
    return 3;
}

QVariant EventDatabase::data(const QModelIndex &index, int role) const
{
    assert(!index.parent().isValid());
    int row = index.row();
    switch (index.column())
    {
    case 0:
        switch (role)
        {
        case Qt::DisplayRole:
            return Event::typeString( m_items[row]->type(), true );
        case Qt::EditRole:
            return (int) m_items[row]->type();
        default:
            return QVariant();
        }
        break;
    case 1:
        switch (role)
        {
        case Qt::DisplayRole:
            return m_items[row]->beginning();
        case Qt::EditRole:
            return QVariant::fromValue( TimeSpan( m_items[row]->timeSpan() ));
        default:
            return QVariant();
        }
        break;
    case 2:
        switch (role)
        {
        case Qt::DisplayRole:
        case Qt::EditRole:
            return m_items[row]->label();
        default:
            return QVariant();
        }
        break;
    default:
        return QVariant();
    }
}

QVariant EventDatabase::headerData(int section, Qt::Orientation orientation, int role) const
{
    Q_UNUSED(section);
    Q_UNUSED(orientation);
    Q_UNUSED(role);
    return QVariant();  // no header
}

Qt::ItemFlags EventDatabase::flags(const QModelIndex &index) const
{
    Q_UNUSED(index);
    return Qt::ItemIsEnabled | Qt::ItemIsEditable | Qt::ItemIsSelectable;
}

void EventDatabase::notifyDataChanged(const QModelIndex &index)
{
    notifyDataChanged(index, index);
}

void EventDatabase::notifyDataChanged(const QModelIndex & start, const QModelIndex & end)
{
    emit dataChanged(start, end);
}

void EventDatabase::notifyDataChanged( const Event *event )
{
    int row = m_items.indexOf((Event*) event);
    if (row < 0)
    {
        return;
    }

    QModelIndex start = index( row, 0, QModelIndex() );
    QModelIndex end   = index( row, columnCount() - 1, QModelIndex() );

    notifyDataChanged( start, end );
}

bool EventDatabase::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (data(index, role) != value)
    {
        app().pushCommand( new EventDatabaseEditEventCommand( this, index, value, role ) );
    }
    return true;
}

bool EventDatabase::setData_(const QModelIndex &index, const QVariant &value, int role)
{
    assert(!index.parent().isValid());

    Event* event = itemAtIndex(index);
    assert( event );
    if (role == Qt::EditRole)
    {
        switch (index.column())
        {
        case 0:
            switch (value.toInt())
            {
            case 0:
                event->setType( Event::Rehearsal );
                break;
            case 1:
                event->setType( Event::Gig );
                break;
            case 2:
                event->setType( Event::Other );
                break;
            default:
                qWarning() << "Did not expect type " << value.toInt();
            }
            break;
        case 1:
            event->setTimeSpan( value.value<TimeSpan>() );
            break;
        case 2:
            event->setLabel( value.toString() );
            break;
        }

        emit dataChanged( index, index );
        return true;
    }

    return false;
}

QVariant EventDatabase::data( const int row, const int column, const int role)
{
    return data( index(row, column, QModelIndex()), role );
}

void EventDatabase::appendEvent(Event *event)
{
    insertEvent(event, rowCount());
}

void EventDatabase::insertEvent(Event *event, const int index)
{
    m_tmpEventBuffer.append(event);
//    connect( song, SIGNAL(attachmentAdded(int)),   this, SIGNAL(attachmentAdded(int)  ));
//    connect( song, SIGNAL(attachmentRemoved(int)), this, SIGNAL(attachmentRemoved(int)));
//    connect( song, SIGNAL(attachmentRenamed(int, QString)), this, SIGNAL(attachmentRenamed(int,QString)));
    assert( insertRows( index, 1, QModelIndex() ));
}

bool EventDatabase::insertRows(int row, int count, const QModelIndex &parent)
{
    assert(!parent.isValid());
    assert( m_tmpEventBuffer.size() == count );

    beginInsertRows(parent, row, row + count - 1);
    for (int i = 0; i < count; ++i)
    {
        m_items.insert( row + i, m_tmpEventBuffer[i] );
        emit eventAdded( row + i, m_tmpEventBuffer[i] );
    }
    m_tmpEventBuffer.clear();
    endInsertRows();

    return true;
}

int EventDatabase::removeEvent(Event* event)
{
    int index;
    if ( (index = m_items.indexOf(event)) < 0 )
    {
        WARNING << "EventDatabase does not contain event " << event;
    }
    else
    {
        assert( removeRows(index, 1, QModelIndex()) );
    }
    return index;
}

bool EventDatabase::removeRows(int row, int count, const QModelIndex &parent)
{
    assert(!parent.isValid());
    assert(m_tmpEventBuffer.isEmpty());

    beginRemoveRows(parent, row, row + count - 1);
    for (int i = 0; i < count; ++i)
    {
        m_items.removeAt(row + i);
        emit eventRemoved(row + i );
    }
    endRemoveRows();

    return true;
}

QModelIndex EventDatabase::indexOfEvent(const Event *event) const
{
    int row = -1;
    for (int i = 0; i < m_items.length() && row < 0; ++i)
    {
        if (event == m_items[i])
        {
            row = i;
        }
    }
    assert( row >= 0 );

    return createIndex( row, 0 );
}

QMimeData* EventDatabase::mimeData(const QModelIndexList &indexes) const
{
    DatabaseMimeData<Event>* mime = new DatabaseMimeData<Event>();
    for (const QModelIndex& index : indexes)
    {
        if (index.column() != 0)
        {
            // we want only one index per row.
            continue;
        }
        mime->append(itemAtIndex(index), index.row());
    }
    return mime;
}

#include "Commands/EventDatabaseCommands/eventdatabaseneweventcommand.h"
bool EventDatabase::dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent)
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
    const DatabaseMimeData<Event>* eventData = DatabaseMimeData<Event>::cast(data);
    if (!eventData)
    {
        return false;
    }

    // copy paste
    if (action == Qt::CopyAction)
    {
        app().beginMacro( tr("Copy events") );
        int i = 0;
        for (DatabaseMimeData<Event>::IndexedItem item : eventData->indexedItems())
        {
            app().pushCommand( new EventDatabaseNewEventCommand( this, item.item->copy(), row + i ) );
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


