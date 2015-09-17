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

QDataStream& operator<<(QDataStream& out, const Section& section)
{
    out << section.m_caption << static_cast<qreal>(section.m_begin) << static_cast<qreal>(section.m_end);
    return out;
}

QDataStream& operator>>(QDataStream& in, Section& section)
{
    qreal begin, end;
    in >> section.m_caption >> begin >> end;
    section.m_begin = begin;
    section.m_end = end;
    return in;
}
