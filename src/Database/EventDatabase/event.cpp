#include "event.h"

const Qt::DateFormat Event::DATE_TIME_FORMAT = Qt::ISODate;

Event::Event( EventDatabase* database, const QDateTime& beginning, const QDateTime& ending, Type type, const QString & label) :
    m_database( database ),
    m_beginning(beginning),
    m_ending(ending),
    m_type(type),
    m_label(label)
{

}

#define TRANSLATE_OPTIONAL( T, STRING ) ( T ? tr(STRING) : STRING )

QString Event::typeString(Type type, bool translated)
{
    switch (type)
    {
    case Rehearsal:
        return TRANSLATE_OPTIONAL( translated, "Rehearsal" );
    case Gig:
        return TRANSLATE_OPTIONAL( translated, "Gig" );
    case Other:
        return TRANSLATE_OPTIONAL( translated, "Other" );
    default:
        assert(false);
        return "";
    }
}

bool Event::restoreFromJsonObject(const QJsonObject &json)
{
    bool success = true;
    if (    checkJsonObject( json, "beginning", QJsonValue::String )
         && checkJsonObject( json, "ending",    QJsonValue::String )
         && checkJsonObject( json, "type",      QJsonValue::String )
         && checkJsonObject( json, "label",     QJsonValue::String ) )
    {
        m_beginning = QDateTime::fromString( json["beginning"].toString(), DATE_TIME_FORMAT );
        m_ending    = QDateTime::fromString( json["ending"].toString(),    DATE_TIME_FORMAT );
        m_label     = json["label"].toString();

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
            if (type != "Other")
            {
                success = false;
                qWarning() << "Did not expected value " << type;
            }
            m_type = Other;
        }
    }
    else
    {
        success = false;
    }
    return success;
}

QJsonObject Event::toJsonObject() const
{
    QJsonObject json;

    json["ending"]    =    m_ending.toString( DATE_TIME_FORMAT );
    json["beginning"] = m_beginning.toString( DATE_TIME_FORMAT );
    json["type"]      = typeString(m_type);

    return json;
}






















