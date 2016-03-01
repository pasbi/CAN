#include "eventdatabase.h"
#include "Project/project.h"
#include "Commands/DatabaseCommands/databaseeditcommand.h"
#include <QJsonDocument>
#include "Database/databasemimedata.h"

EventDatabase::EventDatabase(Project *project) :
    Database(project)
{
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
    const Event* event = m_items[index.row()];  //TODO same code as in SongDatabase::data
    const QString key = event->attributeKeys()[index.column()];
    switch (role)
    {
    case Qt::DisplayRole:
    case Qt::ToolTipRole:
        return event->attributeDisplay(key);
    case Qt::EditRole:
        return event->attribute(key);
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

    Event* event = itemAtIndex(index);
    const QString key = event->attributeKeys()[index.column()];

    if (role == Qt::EditRole)
    {
        event->setAttribute(key, value);
        emit dataChanged(index, index);
        return true;
    }
    else
    {
        return false;
    }
}

QString EventDatabase::itemName(int n) const
{
    return tr("Event(s)", "", n);
}


