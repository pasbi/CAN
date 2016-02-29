#include "songdatabase.h"

#include <QJsonDocument>
#include <QSize>

#include "Commands/DatabaseCommands/databasenewitemcommand.h"
#include "Project/project.h"
#include "songdatabasesortproxy.h"
#include "Attachments/attachment.h"
#include "Attachments/AudioAttachment/audioattachment.h"
#include <QPainter>
#include "chord.h"

SongDatabase::SongDatabase(Project *project) :
    Database(project)
{
}

int SongDatabase::columnCount(const QModelIndex &parent) const
{
    assert(!parent.isValid());
    return 9;
}

QString peopleNames(const QStringList& names, const QBitArray& peoples)
{
    QStringList ps;
    for (int i = 0; i < peoples.size(); ++i)
    {
        if (peoples[i])
        {
            ps << names[i];
        }
    }
    return ps.join(", ");
}

QVariant SongDatabase::data(const QModelIndex &index, int role) const
{
    assert(!index.parent().isValid());

    const Song* song = m_items[index.row()];
    const QString key = song->attributes().keys()[index.column()];
    switch (role)
    {
    case Qt::DisplayRole:
    case Qt::ToolTipRole:
        return song->attributeDisplay(key);
    case Qt::EditRole:
        return song->attributes()[key];
    default:
        return QVariant();
    }
}

QIcon buildIcon(QPixmap midiMap, QPixmap songMap)
{
    if (midiMap.isNull())
    {
        return songMap;
    }
    if (songMap.isNull())
    {
        return midiMap;
    }

    QSize singleSize = QSize(qMax(midiMap.width(), songMap.width()), qMax(midiMap.height(), songMap.height()));
    midiMap = midiMap.scaled(singleSize);
    songMap = songMap.scaled(singleSize);

    QPixmap pixmap(QSize(singleSize.width() * 2 + 3, singleSize.height()));
    pixmap.fill(Qt::transparent);
    QPainter painter(&pixmap);
    painter.drawPixmap(0, 0, midiMap);
    painter.drawPixmap(singleSize.width() + 3, 0, songMap);


    return QIcon(pixmap);
}


AudioAttachment::IndexedFileStatus audioAttachmentStatus(const Song* song)
{
    AudioAttachment::IndexedFileStatus best = AudioAttachment::NoAudioAttachment;
    for (const Attachment* a : song->attachments())
    {
        if (a->inherits("AudioAttachment"))
        {
            best = qMax(best, static_cast<const AudioAttachment*>(a)->status());
        }
    }
    return best;
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
            case 3:
                return tr("Key");
            case 4:
                return tr("Labels");
            case 5:
                return tr("State");
            case 6:
                return tr("Solos");
            case 7:
                return tr("Singers");
            case 8:
                return tr("Comments");
            }

        default:
            return QVariant();
        }
    }
    else
    {
        QPixmap midiIcon, songIcon;
        switch (role)
        {
        case Qt::SizeHintRole:
            return QSize(24, 0);
        case Qt::DecorationRole:
#ifdef HAVE_PROGRAM
            if (m_items[section]->program().isValid())
            {
                midiIcon = QPixmap(":/icons/icons/midi.png");
            }
#endif
            switch (audioAttachmentStatus(m_items[section]))
            {
            case IndexedFileAttachment::NoAudioAttachment:
                songIcon = QPixmap();
                break;
            case IndexedFileAttachment::NoFileSet:
                songIcon = QPixmap(":/icons/icons/songNoFile.png");
                break;
            case IndexedFileAttachment::FileNotAvailable:
                songIcon = QPixmap(":/icons/icons/songNotAvailable.png");
                break;
            case IndexedFileAttachment::FileAvailable:
                songIcon = QPixmap(":/icons/icons/song1.png");
                break;
            }
            return buildIcon(midiIcon, songIcon);
        }
        return QVariant();

    }
}

Qt::ItemFlags SongDatabase::flags(const QModelIndex &index) const
{
    Q_UNUSED(index);
    Qt::ItemFlags flags = Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsDragEnabled;
    if (index.column() < 6)
    {
        flags |= Qt::ItemIsEditable;
    }
    else
    {
        // other columns use dialogs for editing
    }
    return flags;
}

QList<int> toIntList(const QVariant& value)
{
    QList<int> ints;
    for (const QVariant& val : value.toList())
    {
        ints << val.toInt();
    }
    return ints;
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
            song->setDuration(value.toTime());
            break;
        case 3:
            song->setKey(value.value<Chord>());
            break;
        case 4:
            song->setlabel(static_cast<Song::Label>(value.toInt()));
            break;
        case 5:
            song->setState(static_cast<Song::State>(value.toInt()));
            break;
        case 6:
            song->setSoloPlayers(value.toStringList());
            break;
        case 7:
            song->setSingers(value.toStringList());
            break;
        case 8:
            song->setComments(value.toString());
            break;
        default:
            return false;
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

void SongDatabase::serialize(QDataStream &out) const
{
    out << m_peoples;
    Database::serialize(out);
}

void SongDatabase::deserialize(QDataStream &in)
{
    in >> m_peoples;
    Database::deserialize(in);
}

QStringList SongDatabase::peoples() const
{
    QStringList peoples;
    for (const Song* song : items())
    {
        peoples << song->singers();
        peoples << song->soloPlayers();
    }
    peoples.removeDuplicates();
    return peoples;
}
