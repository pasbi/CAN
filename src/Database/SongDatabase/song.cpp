#include "song.h"
#include "songdatabase.h"
#include "global.h"
#include <QJsonDocument>
#include "Attachments/attachment.h"


Song::Song(Database<Song> * database) :
    DatabaseItem(database),
    m_attributes({"title", "artist", "duration", "key", "label", "state", "singers", "soloPlayers", "comments", "creationDateTime"})
{
    m_attributes.set("creationDateTime", QDateTime::currentDateTime());
    m_attributes.set("label", NoLabel);
    m_attributes.set("state", NoState);
    m_attributes.set("key", QVariant::fromValue(Chord()));
    connect( this, SIGNAL(attachmentAdded(int)),   database, SIGNAL(attachmentAdded(int)  ));
    connect( this, SIGNAL(attachmentRemoved(int)), database, SIGNAL(attachmentRemoved(int)));
    connect( this, SIGNAL(attachmentRenamed(int, QString)), database, SIGNAL(attachmentRenamed(int, QString)));
}

Song::~Song()
{
    qDeleteAll( m_attachments );
}

QStringList Song::attachmentNames() const
{
    QStringList akk;
    for (const Attachment* a : m_attachments)
    {
        akk << a->name();
    }
    return akk;
}


int Song::removeAttachment( Attachment* attachment )
{
    int index = m_attachments.indexOf( attachment );
    m_attachments.removeOne( attachment );
    emit attachmentRemoved( index );
    return index;
}

void Song::addAttachment( Attachment* attachment )
{
    int index = m_attachments.length();
    insertAttachment( attachment, index );
}

void Song::insertAttachment(Attachment *attachment, int index)
{
    assert( attachment->song() == this );
    m_attachments.insert( index, attachment );
    connectAttachment( attachment );
    emit attachmentAdded(index);
}

void Song::connectAttachment(Attachment *attachment)
{
    connect( attachment, &Attachment::attachmentRenamed, [this, attachment](QString name)
    {
        emit attachmentRenamed( attachments().indexOf(attachment), name );
    });
}

#define SET_ATTRIBUTE(name) \
    if (name != Song::name()) \
    { \
        m_attributes.set(#name, QVariant::fromValue(name)); \
        emit attributeChanged(); \
    }

void Song::setTitle(const QString& title)
{
    SET_ATTRIBUTE(title)
}

void Song::setArtist(const QString& artist)
{
    SET_ATTRIBUTE(artist)
}

void Song::setDuration(const QTime& duration)
{
    SET_ATTRIBUTE(duration)
}

void Song::setlabel(Label label)
{
    SET_ATTRIBUTE(label)
}

void Song::setState(State state)
{
    SET_ATTRIBUTE(state)
}

void Song::setSingers(const QStringList& singers)
{
    SET_ATTRIBUTE(singers)
}

void Song::setSoloPlayers(const QStringList& soloPlayers)
{
    SET_ATTRIBUTE(soloPlayers)
}

void Song::setComments(const QString& comments)
{
    SET_ATTRIBUTE(comments)
}

void Song::setKey(Chord key)
{
    SET_ATTRIBUTE(key)
}

QString Song::title() const
{
    return m_attributes["title"].toString();
}

QString Song::artist() const
{
    return m_attributes["artist"].toString();
}

QTime Song::duration() const
{
    return m_attributes["duration"].toTime();;
}

QString Song::description() const
{
    return m_attributes["description"].toString();
}

QDateTime Song::creationTime() const
{
    return m_attributes["creationTime"].toDateTime();
}

Song::Label Song::label() const
{
    //TODO
    return Song::Acoustic;
   // return m_attributes.value<Label>("label");
}

Song::State Song::state() const
{
    //TODO
    return Song::Works;
    //return m_attributes.value<State>("state");
}

QStringList Song::singers() const
{
    return m_attributes["singers"].toStringList();
}

QStringList Song::soloPlayers() const
{
    return m_attributes["soloPlayers"].toStringList();
}

#include "chord.h"
Chord Song::key() const
{
    return m_attributes["key"].value<Chord>();
}

QString Song::comments() const
{
    return m_attributes["comments"].toString();
}


QStringList Song::textAttributes() const
{
    return QStringList( { artist(), title() } );
}

void Song::deserialize(QDataStream &in)
{
    //TODO
    DatabaseItem::deserialize(in);
    qint32 n, state, label;
    QString title, artist, comments;
    QStringList singers, soloPlayers;
    QTime duration;
    QDateTime creationDateTime;
    Chord key;

    in >> title >> artist >> creationDateTime >> duration;
    in >> state >> label >> key >> singers >> soloPlayers >> comments;
    m_attributes.set("title", title);
    m_attributes.set("artist", artist);
    m_attributes.set("duration", duration);
    m_attributes.set("creationDateTime", creationDateTime);
    m_attributes.set("label", static_cast<Label>(label));
    m_attributes.set("state", static_cast<State>(state));
    m_attributes.set("singers", singers);
    m_attributes.set("soloPlayers", soloPlayers);
    m_attributes.set("comments", comments);
    m_attributes.set("key", QVariant::fromValue(key));
    in >> &m_program;
    in >> n;
    assert(m_attachments.isEmpty());
    m_attachments.reserve(n);
    for (int i = 0; i < n; ++i)
    {
        QString classname;
        in >> classname;
        Attachment* attachment = Attachment::create(classname, this);
        in >> attachment;
        m_attachments.append( attachment );
        connectAttachment(attachment);
    }

}

void Song::serialize(QDataStream &out) const
{
    DatabaseItem::serialize(out);
    out << m_attributes["title"].toString();
    out << m_attributes["artist"].toString();
    out << m_attributes["creationDateTime"].toString();
    out << m_attributes["duration"].toString();
    out << static_cast<qint32>(m_attributes["state"].value<State>());
    out << static_cast<qint32>(m_attributes["label"].value<Label>());
    out << m_attributes["key"].value<Chord>();
    out << m_attributes["singers"].toStringList();
    out << m_attributes["soloPlayers"].toStringList();
    out << m_attributes["comments"].toString();
    out << &m_program;
    out << static_cast<qint32>(m_attachments.length());
    for (const Attachment* a : m_attachments)
    {
        out << a->type();
        out << a;
    }
}

PedanticVariantMap Song::attributes() const
{
    return m_attributes;
}

QString Song::attributeDisplay(const QString &key) const
{
    if (key == "label")
    {
        return Song::labelNames()[static_cast<int>(label())];
    }
    if (key == "state")
    {
        return Song::stateNames()[static_cast<int>(state())];
    }
    if (key == "key")
    {
        return Song::key().key();
    }
    if (key == "duration")
    {
        return Song::duration().toString("mm:ss");
    }
    if (m_attributes[key].canConvert(QVariant::StringList))
    {
        return m_attributes[key].toStringList().join(", ");
    }
    if (m_attributes[key].canConvert(QVariant::String))
    {
        return m_attributes[key].toString();
    }
    Q_UNREACHABLE();
    return QString();
}


void Song::setAttributes(const PedanticVariantMap &attributes)
{
    m_attributes = attributes;
    emit attributeChanged();
}


QStringList Song::labelNames()
{
    return QStringList({ tr(""), tr("Acoustic"), tr("Normal"), tr("A/N")});
}

QStringList Song::stateNames()
{
    return QStringList({ tr(""), tr("Inactive"), tr("Needs Practice"), tr("Works") });
}
