#include "setlistitem.h"
#include "application.h"
#include "Project/project.h"
#include "Database/SongDatabase/songdatabase.h"
#include "setlist.h"

SetlistItem::SetlistItem( Setlist* setlist, const QString & label ) :
    m_type( LabelType ),
    m_label( label ),
    m_setlist(setlist)
{
}

SetlistItem::SetlistItem( Setlist* setlist ) :
    m_type( LabelType ),
    m_label( QObject::tr("Unnamed") ),
    m_setlist(setlist)
{
}

SetlistItem::SetlistItem( Setlist* setlist, const Song* song ) :
    m_type( SongType ),
    m_song( song ),
    m_setlist(setlist)
{
    QObject::connect(song, &Song::attributeChanged, [song, this]()
    {
        QModelIndex index = m_setlist->indexOf(this);
        emit m_setlist->dataChanged(index, index);
    });
}

SetlistItem::~SetlistItem()
{
    if (m_song)
    {
        m_song->disconnect(m_setlist);
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

SetlistItem* SetlistItem::fromJson( const QJsonObject & object, Setlist* setlist )
{
    if (!checkJsonObject( object, "type", QJsonValue::Double ))
    {
        return NULL;
    }
    switch ((Type) object["type"].toInt())
    {
    case SongType:
        if (!checkJsonObject(object, "SongID", QJsonValue::String))
        {
            return NULL;
        }
        else
        {
            Song* song = app().project()->songDatabase()->song( object["SongID"].toString() );
            if (song)
            {
                return new SetlistItem( setlist, song );
            }
            else
            {
                return NULL;
            }
        }
    case LabelType:
        if (!checkJsonObject(object, "Label", QJsonValue::String))
        {
            return NULL;
        }
        else
        {
            return new SetlistItem( setlist, object["Label"].toString() );
        }
    }
    return NULL;
}

SetlistItem* SetlistItem::copy() const
{
    return fromJson(toJson(), m_setlist);
}

