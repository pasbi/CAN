#include "setlistitem.h"
#include "application.h"
#include "Project/project.h"
#include "Database/SongDatabase/songdatabase.h"
#include "Database/EventDatabase/setlist.h"


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

QJsonObject SetlistItem::toJsonObject() const
{
    QJsonObject object;

    object["type"] = (int) m_type;
    if (m_song)
    {
        object["SongID"] = m_song->randomID();
    }
    else
    {
        object["SongID"] = "";
    }
    object["Label"] = m_label;

    return object;
}

bool SetlistItem::restoreFromJsonObject(const QJsonObject & object)
{
    if (!checkJsonObject( object, "type", QJsonValue::Double ))
    {
        return false;
    }
    switch (static_cast<Type>(object["type"].toInt()))
    {
    case SongType:
        if (!checkJsonObject(object, "SongID", QJsonValue::String))
        {
            qWarning() << "Did not found SongID";
            return false;
        }
        else
        {
            Song* song = app().project()->songDatabase()->song( object["SongID"].toString() );
            if (song)
            {
                m_type = SongType;
                setSong(song);
                return true;
            }
            else
            {
                qWarning() << "Failed to resolve song";
                return false;
            }
        }
        break;
    case LabelType:
        if (!checkJsonObject(object, "Label", QJsonValue::String))
        {
            qWarning() << "Did not found label";
            return false;
        }
        else
        {
            m_type = LabelType;
            m_label = object["Label"].toString();
            return true;
        }
    }
    return false;
}

QStringList SetlistItem::textAttributes() const
{
    return QStringList( { label() } );
}

