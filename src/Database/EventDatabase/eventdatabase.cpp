#include "eventdatabase.h"
#include "project.h"
#include "Commands/EventDatabaseCommands/eventdatabaseediteventcommand.h"

EventDatabase::EventDatabase(Project *project) :
    Database(project)
{
}


QJsonObject EventDatabase::toJsonObject() const
{
    QJsonObject json;

    json["numEvents"] = m_events.length();
    for (int i = 0; i < m_events.length(); ++i)
    {
        m_events[i]->saveTo( project()->makeAbsolute( QString("event%1").arg(i) ) );
    }

    return json;
}

bool EventDatabase::restoreFromJsonObject(const QJsonObject &object)
{
    beginResetModel();
    bool success = true;
    success &= checkJsonObject( object, "numEvents", QJsonValue::Double );
    int numEvents = object["numEvents"].toInt();

    m_events.clear();
    for (int i = 0; i < numEvents; ++i)
    {
        Event* e = new Event( this );
        success &= e->loadFrom( project()->makeAbsolute( QString("event%1").arg(i) ) );
        m_events << e;
    }

    endResetModel();
    return success;
}

void EventDatabase::reset()
{

}

int EventDatabase::rowCount(const QModelIndex &parent) const
{
    assert( !parent.isValid() );
    return m_events.length();
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
            return Event::typeString( m_events[row]->type(), true );
        case Qt::EditRole:
            return (int) m_events[row]->type();
        default:
            return QVariant();
        }
        break;
    case 1:
        switch (role)
        {
        case Qt::DisplayRole:
        case Qt::EditRole:
            return m_events[row]->beginning();
        default:
            return QVariant();
        }
        break;
    case 2:
        switch (role)
        {
        case Qt::DisplayRole:
        case Qt::EditRole:
            return m_events[row]->label();
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
    int row = m_events.indexOf((Event*) event);
    if (row < 0)
    {
        return;
    }

    QModelIndex start = index( row, 0, QModelIndex() );
    QModelIndex end   = index( row, columnCount() - 1, QModelIndex() );

    notifyDataChanged( start, end );
}

Event* EventDatabase::eventAtIndex(const QModelIndex & index) const
{
    if (index.isValid() && index.row() < rowCount())
    {
        return m_events.at(index.row());
    }
    else
    {
        return NULL;
    }
}

bool EventDatabase::setData(const QModelIndex &index, const QVariant &value, int role)
{
    project()->pushCommand( new EventDatabaseEditEventCommand( this, index, value, role ) );
    return true;
}

bool EventDatabase::setData_(const QModelIndex &index, const QVariant &value, int role)
{
    assert(!index.parent().isValid());

    Event* event = eventAtIndex(index);
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
            event->setBeginning( value.toDateTime() );
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
        m_events.insert( row + i, m_tmpEventBuffer[i] );
        emit eventAdded( row + i, m_tmpEventBuffer[i] );
    }
    m_tmpEventBuffer.clear();
    endInsertRows();

    return true;
}

int EventDatabase::removeEvent(Event* event)
{
    int index;
    if ( (index = m_events.indexOf(event)) < 0 )
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
        m_events.removeAt(row + i);
        emit eventRemoved(row + i );
    }
    endRemoveRows();

    return true;
}


