#include "event.h"
#include "setlist.h"

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

#define OLD_LOAD //TODO
#define OLD_SAVE

void Event::serialize(QDataStream &out) const
{
#ifdef OLD_SAVE
    out << QDateTime(); //LEGACY
    out << attribute("beginning").toDateTime();
    out << static_cast<EnumSurrogate_t>(attribute("type").value<Type>());
    out << attribute("label").toString();
#else
    DatabaseItem::serialize(out);
#endif
    out << m_setlist;
}

void Event::deserialize(QDataStream &in)
{
#ifdef OLD_LOAD
    EnumSurrogate_t type;
    QDateTime unusedEndingDateTime;
    QDateTime beginning;
    QString label;

    in >> unusedEndingDateTime; //LEGACY
    in >> beginning;
    in >> type;
    in >> label;
    setAttribute("beginning", beginning);
    setAttribute("type", QVariant::fromValue<Type>(static_cast<Type>(type)));
    setAttribute("label", label);
#else
    DatabaseItem::deserialize(in);
#endif
    qDebug() << "deserialize setlist";
    in >> m_setlist;

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
        const QString format = tr("MM/dd/yy hh:mm ap");
        return attribute.toDateTime().toString(format);
        return attribute.toDateTime().toString(format);
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

QDataStream& operator <<(QDataStream& out, const Event::Type& type)
{
    out << static_cast<EnumSurrogate_t>(type);
    return out;
}

QDataStream& operator >>(QDataStream& in,        Event::Type& type)
{
    EnumSurrogate_t ftype;
    in >> ftype;
    type = static_cast<Event::Type>(ftype);
    return in;
}






