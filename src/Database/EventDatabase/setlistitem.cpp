#include "setlistitem.h"
#include "application.h"
#include "Project/project.h"
#include "Database/SongDatabase/songdatabase.h"
#include "Database/EventDatabase/setlist.h"

const QString SetlistItem::TYPE_NAME = SetlistItem::tr("SetlistItem");


SetlistItem::SetlistItem(Database<SetlistItem> *setlist, const QString & label ) :
    DatabaseItem(setlist),
    m_type( LabelType ),
    m_label( label )
{
}

SetlistItem::SetlistItem( Database<SetlistItem>* setlist ) :
    DatabaseItem(setlist),
    m_type( LabelType ),
    m_label( QObject::tr("Unnamed") )
{
}

SetlistItem::SetlistItem( Database<SetlistItem>* setlist, const Song* song ) :
    DatabaseItem(setlist),
    m_type( SongType )
{
    setSong(song);
}

SetlistItem::~SetlistItem()
{
    if (m_song)
    {
        QObject::disconnect(m_updateSongLabelConnection);
    }
}

void SetlistItem::setSong(const Song *song)
{
    if (m_song)
    {
        QObject::disconnect(m_updateSongLabelConnection);
    }

    m_song = song;

    m_updateSongLabelConnection = QObject::connect(song, &Song::attributeChanged, [song, this]()
    {
        int row = database()->rowOf(this);
        if (row >= 0)
        {
            QModelIndex index = database()->index(row, 0);
            emit database()->dataChanged(index, index);
        }
    });
}

QString SetlistItem::labelSong(const Song *song)
{
    return QString("%1 - %2").arg(song->title(), song->artist());
}

QString SetlistItem::label() const
{
    switch (m_type)
    {
    case SongType:
        return labelSong(m_song);
    case LabelType:
        return m_label;
    default:
        return QString();
    }
}

bool SetlistItem::setLabel(const QString label)
{
    if (type() == LabelType)
    {
        m_label = label;
        return true;
    }
    else
    {
        // ignore.
        return false;
    }
}

void SetlistItem::serialize(QDataStream &out) const
{
    DatabaseItem::serialize(out);
    out << static_cast<qint32>(m_type);
    if (m_type == SongType)
    {
        out << static_cast<qint32>(app().project()->songDatabase()->identifyItem(m_song));
    }
    else
    {
        out << m_label;
    }
}

void SetlistItem::deserialize(QDataStream &in)
{
    DatabaseItem::deserialize(in);
    qint32 type;
    in >> type;
    m_type = static_cast<Type>(type);
    if (m_type == SongType)
    {
        qint32 id;
        in >> id;
        m_song = app().project()->songDatabase()->retrieveItem(id);
    }
    else
    {
        in >> m_label;
    }
}

QStringList SetlistItem::textAttributes() const
{
    return QStringList( { label() } );
}

