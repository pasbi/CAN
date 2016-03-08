#include "eventdatabase.h"
#include "Project/project.h"
#include "Commands/DatabaseCommands/databaseeditcommand.h"
#include <QJsonDocument>
#include "Database/databasemimedata.h"
#include "Database/EventDatabase/setlist.h"

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

QString EventDatabase::itemName(int n) const
{
    return tr("Event(s)", "", n);
}
