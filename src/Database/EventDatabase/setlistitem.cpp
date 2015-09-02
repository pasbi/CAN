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
    m_type( SongType ),
    m_song( song )
{
    QObject::connect(song, &Song::attributeChanged, [song, this]()
    {
        int row = database()->rowOf(this);
        QModelIndex index = database()->index(row, 0);
        emit database()->dataChanged(index, index);
    });
}

SetlistItem::~SetlistItem()
{
    if (m_song)
    {
        m_song->disconnect(database());
    }
}

QString SetlistItem::label() const
{
    switch (m_type)
    {
    case SongType:
        return QString("%1 - %2").arg(m_song->title(), m_song->artist());
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

QJsonObject SetlistItem::toJson() const
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
    switch ((Type) object["type"].toInt())
    {
    case SongType:
        if (!checkJsonObject(object, "SongID", QJsonValue::String))
        {
            return false;
        }
        else
        {
            Song* song = app().project()->songDatabase()->song( object["SongID"].toString() );
            if (song)
            {
                m_song = song;
            }
            else
            {
                return false;
            }
        }
    case LabelType:
        if (!checkJsonObject(object, "Label", QJsonValue::String))
        {
            return false;
        }
        else
        {
            m_label = object["Label"].toString();
        }
    }
    return false;
}

