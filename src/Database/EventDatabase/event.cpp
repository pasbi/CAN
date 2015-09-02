#include "event.h"
#include "setlist.h"

const QStringList Event::TYPES = QStringList({ Event::tr("Rehearsal"), Event::tr("Gig"), Event::tr("Other") });

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

bool Event::restoreFromJsonObject(const QJsonObject &json)
{
    bool success = true;
    if (    checkJsonObject( json, "beginning", QJsonValue::String )
         && checkJsonObject( json, "ending",    QJsonValue::String )
         && checkJsonObject( json, "label",     QJsonValue::String )
         && checkJsonObject( json, "notices",   QJsonValue::String )
         && checkJsonObject( json, "setlist",   QJsonValue::Array ) )
    {
        m_timeSpan  = TimeSpan( QDateTime::fromString( json["beginning"].toString(), DATE_TIME_FORMAT ),
                                QDateTime::fromString( json["ending"].toString(),    DATE_TIME_FORMAT ) );
        m_label     = json["label"].toString();
        m_notices   = json["notices"].toString();
        m_setlist->restoreFromJsonObject( json );

    }
    else
    {
        return false;
    }

    if ( checkJsonObject( json, "type", QJsonValue::Double ))
    {
        // Load the new format where Event::type is encoded as int
        m_type = static_cast<Type>(json["type"].toDouble());
    }
    else if checkJsonObject( json, "type", QJsonValue::String)
    {
        qWarning() << "load deprecated Event-format.";
        QString type = json["type"].toString();
        if (type == "Rehearsal")
        {
            m_type = Rehearsal;
        }
        else if (type == "Gig")
        {
            m_type = Gig;
        }
        else
        {
            m_type = Other;
        }
    }


    if (!checkJsonObject( json, "id", QJsonValue::String))
    {
        qWarning() << "expected id. create one: " << randomID();
    }
    else
    {
        m_randomID = json["id"].toString();
    }

    return success;
}

QJsonObject Event::toJsonObject() const
{
    QJsonObject json;

    json["ending"]    = m_timeSpan.ending.toString( DATE_TIME_FORMAT );
    json["beginning"] = m_timeSpan.beginning.toString( DATE_TIME_FORMAT );
    json["type"]      = static_cast<int>(m_type);
    json["label"]     = m_label;
    json["notices"]   = m_notices;
    json["setlist"]   = m_setlist->toJsonObject()["setlist"];
    json["id"]        = randomID();

    return json;
}

void Event::setNotice(const QString &notice)
{
    m_notices = notice;
}

QString Event::description() const
{
    if (label().isEmpty())
    {
        return typeName( type() );
    }
    else
    {
        return label();
    }
}

QString Event::typeName(Type type)
{
    return TYPES[(int) type];
}

QStringList Event::textAttributes() const
{
    return QStringList( { label(), notices() } );
}












