#include "event.h"
#include "setlist.h"


Event::Event( Database<Event>* database, const QDateTime& beginning, const QDateTime& ending, Type type, const QString & label) :
    DatabaseItem( database ),
    m_timeSpan( TimeSpan(beginning, ending)),
    m_type(type),
    m_label(label),
    m_setlist( new Setlist(this) )
{

}

Event::~Event()
{
    delete m_setlist;
    m_setlist = nullptr;
}

void Event::setLabel( const QString & label )
{
    m_label = label;
}

void Event::setBeginning( const QDateTime & beginning )
{
    m_timeSpan.beginning = beginning;
}

void Event::setEnding( const QDateTime & ending )
{
    m_timeSpan.ending = ending;
}

void Event::setTimeSpan( const TimeSpan& timeSpan)
{
    m_timeSpan = timeSpan;
}

void Event::setType( Type type )
{
    m_type = type;
}

void Event::setNotice(const QString &notice)
{
    m_notices = notice;
}

QString Event::description() const
{
    if (label().isEmpty())
    {
        return eventTypeName( type() );
    }
    else
    {
        return label();
    }
}

QString Event::eventTypeName(Type type)
{
    return eventTypeNames()[static_cast<int>(type)];
}

QStringList Event::textAttributes() const
{
    return QStringList( { eventTypeName(type()), label(), notices() } );
}

void Event::serialize(QDataStream &out) const
{
    DatabaseItem::serialize(out);
    out << m_timeSpan.ending;
    out << m_timeSpan.beginning;
    out << static_cast<qint32>(m_type);
    out << m_label;
    out << m_setlist;
}

void Event::deserialize(QDataStream &in)
{
    DatabaseItem::deserialize(in);
    qint32 type;
    in >> m_timeSpan.ending;
    in >> m_timeSpan.beginning;
    in >> type;
    in >> m_label;
    in >> m_setlist;

    m_type = static_cast<Event::Type>(type);
}

QStringList Event::eventTypeNames()
{
    return QStringList({ app().translate("Event", "Rehearsal"), app().translate("Event", "Gig"), app().translate("Event", "Other") });
}







