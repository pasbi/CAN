#include "songdatabase.h"

#include <QJsonDocument>
#include <QSize>

#include "Commands/DatabaseCommands/databasenewitemcommand.h"
#include "Project/project.h"
#include "songdatabasesortproxy.h"
#include "application.h"
#include "commontypes.h"
#include "Attachments/attachment.h"
#include "Attachments/AudioAttachment/audioattachment.h"
#include <QPainter>

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
