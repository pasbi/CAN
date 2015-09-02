#include "eventdatabase.h"
#include "Project/project.h"
#include "Commands/DatabaseCommands/databaseedititemcommand.h"
#include "application.h"
#include <QJsonDocument>
#include "Database/databasemimedata.h"

EventDatabase::EventDatabase(Project *project) :
    Database<Event>(project)
{
    Event::seedRandomID();
}

Qt::DropActions EventDatabase::supportedDragActions() const
{
    return Qt::IgnoreAction;
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
            return Event::typeName( m_items[row]->type() );
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

bool EventDatabase::setData(const QModelIndex &index, const QVariant &value, int role)
{
    assert(!index.parent().isValid());

    Event* event = resolveItemAtIndex(index);
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



