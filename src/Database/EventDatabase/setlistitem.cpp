#include "setlistitem.h"
#include "application.h"
#include "Project/project.h"

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

SetlistItem* SetlistItem::fromJson( const QJsonObject & object )
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
                return new SetlistItem( song );
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
            return new SetlistItem( object["Label"].toString() );
        }
    }
    return NULL;
}

SetlistItem* SetlistItem::copy() const
{
    return fromJson(toJson());
}

QDataStream& operator << (QDataStream& out, const SetlistItem* item )
{
    out << (quint16) item->m_type;
    switch (item->m_type)
    {
    case SetlistItem::SongType:
        out << (qintptr) item->m_song;
        break;
    case SetlistItem::LabelType:
        out << item->m_label;
        break;
    }
    return out;
}

QDataStream& operator >> (QDataStream& in,  SetlistItem* &item )
{
    quint16 type;
    in >> type;
    switch ((SetlistItem::Type) type)
    {
    case SetlistItem::SongType:
    {
        qintptr ptr;
        in >> ptr;
        item = new SetlistItem( (Song*) ptr );
        break;
    }
    case SetlistItem::LabelType:
    {
        QString string;
        in >> string;
        item = new SetlistItem( string );
        break;
    }
    default:
        item = NULL;
    }
    return in;
}

