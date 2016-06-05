#include "section.h"
#include <qmath.h>
#include "jarowinkler.h"

double timeToDouble(const QTime& time)
{
    return QTime(0, 0).msecsTo(time) / 1000.0;
}

QTime doubleToTime(const double secs)
{
    return QTime::fromMSecsSinceStartOfDay(secs * 1000.0);
}

Section::Section( const QString & caption, const QTime& begin, const QTime& end ) :
    m_caption( caption ),
    m_begin( begin ),
    m_end( end )
{
    Q_ASSERT( begin.msecsTo(doubleToTime(timeToDouble(begin))) < 1000);
    Q_ASSERT( end.msecsTo(doubleToTime(timeToDouble(end))) < 1000);

    if (m_begin > m_end)
    {
        qSwap( m_begin, m_end );
        qWarning() << "Beginning is after ending. Swapped them.";
    }
}

Section::Section( const QString & caption, double begin, double end ) :
    Section(caption, doubleToTime(begin), doubleToTime(end))
{
    Q_ASSERT( qFuzzyCompare(begin, timeToDouble(doubleToTime(begin))) );
    Q_ASSERT( qFuzzyCompare(end, timeToDouble(doubleToTime(end))) );
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

bool Section::operator ==(const Section& other) const
{
    return other.begin() == begin() && other.end() == end() && other.caption() == caption();
}

bool Section::operator !=(const Section& other) const
{
    return !(*this == other);
}

QDataStream& operator<<(QDataStream& out, const Section& section)
{
    out << section.m_caption << section.m_begin << section.m_end;
    return out;
}

QDataStream& operator>>(QDataStream& in, Section& section)
{
    in >> section.m_caption >> section.m_begin >> section.m_end;

    return in;
}

bool Section::isValid() const
{
    return m_begin.isValid() && m_end.isValid();
}

Ratio Section::similarity(const Section *other) const
{
    Ratio r;
    r += jaro_winkler_distance(m_caption, other->m_caption);
    r += other->begin() == begin() ? 1 : 0;
    r +=  other->end()  ==  end()  ? 1 : 0;
    return r;
}

