#include "section.h"
#include "persistentobject.h"
#include <qmath.h>

double timeToDouble(const QTime& time)
{
    double t = 0;
    t += 60 * 60 * time.hour();
    t += 60 * time.minute();
    t += time.second();
    t += time.msec() / 60.0;
    return t;
}

QTime doubleToTime(const double time)
{
    int msecs = int(time * 1000) % 1000;
    int secs = int(time) % 60;
    int mins = int(time / 60) % 60;
    int hours = int(time / (60 * 60)) % 60;
    return QTime(hours, mins, secs, msecs);
}

Section::Section( const QString & caption, const QTime& begin, const QTime& end ) :
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

Section::Section( const QString & caption, const double begin, const double end ) :
    m_caption( caption ),
    m_begin( doubleToTime(begin) ),
    m_end( doubleToTime(end) )
{
    if (m_begin > m_end)
    {
        qSwap( m_begin, m_end );
        qWarning() << "Beginning is after ending. Swapped them.";
    }
}


Section::Section() :
    m_caption( QString() ),
    m_begin( QTime() ),
    m_end( QTime() )
{
}

double Section::beginDouble() const
{
    return timeToDouble(begin());
}

double Section::endDouble() const
{
    return timeToDouble(end());
}

void reorder(QTime& begin, QTime& end)
{
    if (begin >= end)
    {
        QTime aux = end;
        end = begin;
        begin = aux;
    }
}

void Section::setBegin(const QTime &begin)
{
    m_begin = begin;
    reorder(m_begin, m_end);
}

void Section::setEnd(const QTime &end)
{
    m_end = end;
    reorder(m_begin, m_end);
}

QDataStream& operator<<(QDataStream& out, const Section& section)
{
    out << section.m_caption << section.m_begin << section.m_end;
    return out;
}

QDataStream& operator>>(QDataStream& in, Section& section)
{
    QTime begin, end;
    in >> section.m_caption >> begin >> end;

    section.m_begin = begin;
    section.m_end = end;
    return in;
}


