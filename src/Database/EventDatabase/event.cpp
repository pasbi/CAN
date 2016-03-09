#include "event.h"
#include "setlist.h"
#include "application.h"
#include "setlistitem.h"

const QStringList Event::ATTRIBUTE_KEYS = { "type", "beginning", "label", "notices" };

Event::Event( Database<Event>* database, const QDateTime& beginning, Type type, const QString & label) :
    DatabaseItem( ATTRIBUTE_KEYS, database ),
    m_setlist( new Setlist(this) )
{
    setAttribute("beginning", beginning);
    setAttribute("type", type);
    setAttribute("label", label);
}

Event::~Event()
{
    delete m_setlist;
    m_setlist = nullptr;
}

QString Event::description() const
{
    if (attributeDisplay("label").isEmpty())
    {
        return attributeDisplay("type");
    }
    else
    {
        return attributeDisplay("label");
    }
}

QString Event::eventTypeName(Type type)
{
    return eventTypeNames()[static_cast<int>(type)];
}

QString Event::label() const
{
    QString label = attributeDisplay("label");
    if (!label.isEmpty())
    {
        label = attributeDisplay("type");
    }
    return QString("%1 (%2)").arg(label, attributeDisplay("beginning"));
}

QStringList Event::eventTypeNames()
{
    return QStringList({ app().translate("Event", "Rehearsal"), app().translate("Event", "Gig"), app().translate("Event", "Other") });
}

QString Event::attributeDisplay(const QString &key) const
{
    QVariant attribute = DatabaseItem::attribute(key);
    if (key == "beginning" || key == "creationDateTime")
    {
        return attribute.toDateTime().toString(preference<QString>("dateTimeFormat"));
        return attribute.toDateTime().toString(preference<QString>("dateTimeFormat"));
    }
    if (key == "type")
    {
        return eventTypeName(attribute.value<Type>());
    }
    if (attribute.isNull())
    {
        return "";
    }
    if (attribute.canConvert<QString>())
    {
        return attribute.toString();
    }
    Q_UNREACHABLE();
    return "";
}

bool Event::needsSong(const Song *song) const
{
    for (SetlistItem* item : setlist()->items())
    {
        if (item->attribute("song").value<const Song*>() == song)
        {
            return true;
        }
    }
    return false;
}

void Event::serialize(QDataStream& out) const
{
    DatabaseItem::serialize(out);
    out << m_setlist;
}

void Event::deserialize(QDataStream& in)
{
    DatabaseItem::deserialize(in);
    in >> m_setlist;
}

DEFINE_ENUM_STREAM_OPERATORS(Event::Type)






