#include "section.h"
#include "persistentobject.h"

Section::Section( const QString & caption, const double begin, const double end ) :
    m_caption( caption ),
    m_begin( begin ),
    m_end( end )
{
    if (m_begin > m_end)
    {
        qSwap( m_begin, m_end );
        qWarning() << "Beginning is after ending. Swapped them.";
    }
}

Section::Section() :
    m_caption( QString() ),
    m_begin( -1 ),
    m_end( -1 )
{
}

QJsonObject Section::toJsonObject() const
{
    QJsonObject object;
    object["caption"] = m_caption;
    object["begin"] = m_begin;
    object["end"] = m_end;
    return object;
}

bool Section::restoreFromJsonObject(const QJsonObject &object)
{
    if (    checkJsonObject( object, "caption", QJsonValue::String )
         && checkJsonObject( object, "begin", QJsonValue::Double )
         && checkJsonObject( object, "end", QJsonValue::Double )    )
    {
        m_caption = object["caption"].toString();
        m_begin = object["begin"].toDouble();
        m_end = object["end"].toDouble();
        return true;
    }
    else
    {
        return false;
    }
}
