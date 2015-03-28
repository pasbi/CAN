#include "eventdatabase.h"

EventDatabase::EventDatabase(Project *project) :
    Database(project)
{
    m_dates << new Date( this, QDateTime::currentDateTime(), QDateTime::currentDateTime(), Date::Rehearsal, "Generalprobe" );
}


QJsonObject EventDatabase::toJsonObject() const
{
    return QJsonObject();
}

bool EventDatabase::restoreFromJsonObject(const QJsonObject &object)
{
    Q_UNUSED(object);
    return true;
}

void EventDatabase::reset()
{

}

int EventDatabase::rowCount(const QModelIndex &parent) const
{
    assert( !parent.isValid() );
    return m_dates.length();
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
            return Date::typeString( m_dates[row]->type(), true );
        case Qt::EditRole:
            return (int) m_dates[row]->type();
        default:
            return QVariant();
        }
        break;
    case 1:
        switch (role)
        {
        case Qt::DisplayRole:
        case Qt::EditRole:
            return m_dates[row]->beginning();
        default:
            return QVariant();
        }
        break;
    case 2:
        switch (role)
        {
        case Qt::DisplayRole:
        case Qt::EditRole:
            return m_dates[row]->label();
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

void EventDatabase::notifyDataChanged( const Date *date )
{
    int row = m_dates.indexOf((Date*) date);
    if (row < 0)
    {
        return;
    }

    QModelIndex start = index( row, 0, QModelIndex() );
    QModelIndex end   = index( row, columnCount() - 1, QModelIndex() );

    notifyDataChanged( start, end );
}

Date* EventDatabase::dateAtIndex(const QModelIndex & index) const
{
    if (index.isValid() && index.row() < rowCount())
    {
        return m_dates.at(index.row());
    }
    else
    {
        return NULL;
    }
}

bool EventDatabase::setData(const QModelIndex &index, const QVariant &value, int role)
{
    assert(!index.parent().isValid());

    Date* date = dateAtIndex(index);
    assert( date );
    if (role == Qt::EditRole)
    {
        switch (index.column())
        {
        case 0:
            switch (value.toInt())
            {
            case 0:
                date->setType( Date::Rehearsal );
                break;
            case 1:
                date->setType( Date::Gig );
                break;
            case 2:
                date->setType( Date::Other );
                break;
            default:
                qWarning() << "Did not expect type " << value.toInt();
            }
            break;
        case 1:
            date->setBeginning( value.toDateTime() );
            break;
        case 2:
            date->setLabel( value.toString() );
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

