#include "songdatabase.h"

#include <QJsonDocument>
#include <QSize>

#include "Commands/DatabaseCommands/databasenewitemcommand.h"
#include "Project/project.h"
#include "songdatabasesortproxy.h"
#include "application.h"
#include "commontypes.h"

SongDatabase::SongDatabase(Project *project) :
    Database(project)
{
}

int SongDatabase::columnCount(const QModelIndex &parent) const
{
    assert(!parent.isValid());
    return 3;
}

QVariant SongDatabase::data(const QModelIndex &index, int role) const
{
    assert(!index.parent().isValid());

    switch (role)
    {
    case Qt::DisplayRole:
    case Qt::EditRole:
    case Qt::ToolTipRole:
    {
        const Song* song = m_items[index.row()];
        switch (index.column())
        {
        case 0:
            return song->title();
        case 1:
            return song->artist();
        case 2:
            if (role == Qt::DisplayRole)
            {
                return song->duration().toString("mm:ss");
            }
            else
            {
                return song->duration();
            }
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
        case Qt::ToolTipRole:
            switch (section)
            {
            case 0:
                return tr("Title");
            case 1:
                return tr("Artist");
            case 2:
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
    return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsDragEnabled | Qt::ItemIsEditable;
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
        emit dataChanged(index, index);
        return true;
    }

    return false;
}

Qt::DropActions SongDatabase::supportedDragActions() const
{
    return Qt::LinkAction;
}

QString SongDatabase::itemName(int n) const
{
    return tr("%Song(s)", "", n);
}
