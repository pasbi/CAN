#include "song.h"
#include "songdatabase.h"
#include "global.h"
#include <QJsonDocument>
#include "Attachments/attachment.h"

const QStringList Song::ATTRIBUTE_KEYS = {"title", "artist", "duration", "key", "label", "state", "singers", "soloPlayers", "comments", "creationDateTime"};

Song::Song(Database<Song> * database) :
    DatabaseItem(database),
    m_attributes(ATTRIBUTE_KEYS)
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

void Song::setAttribute(const QString &key, const QVariant &value)
{
    m_attributes.set(key, value);
}

void Song::setAttribute(int i, const QVariant &value)
{
    setAttribute(ATTRIBUTE_KEYS[i], value);
}

QVariant Song::attribute(const QString& key) const
{
    return m_attributes[key];
}

QVariant Song::attribute(int i) const
{
    return attribute(ATTRIBUTE_KEYS[i]);
}

QStringList Song::textAttributes() const
{
    return QStringList( { attribute("artist").toString(), attribute("label").toString() } );
}

void Song::deserialize(QDataStream &in)
{
    DatabaseItem::deserialize(in);

#ifdef OLD_LOAD
    qint32 state, label;
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
    m_attributes.set("label", QVariant::fromValue(static_cast<Label>(label)));
    m_attributes.set("state", QVariant::fromValue(static_cast<State>(state)));
    m_attributes.set("singers", singers);
    m_attributes.set("soloPlayers", soloPlayers);
    m_attributes.set("comments", comments);
    m_attributes.set("key", QVariant::fromValue(key));
#else
    in >> m_attributes;
#endif

    in >> &m_program;

    qint32 n;
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

#ifdef OLD_SAVE
    out << m_attributes["title"].toString();
    out << m_attributes["artist"].toString();
    out << m_attributes["creationDateTime"].toDateTime();
    out << m_attributes["duration"].toTime();
    out << static_cast<qint32>(m_attributes["label"].value<Label>());
    out << static_cast<qint32>(m_attributes["state"].value<State>());
    out << m_attributes["key"].value<Chord>();
    out << m_attributes["singers"].toStringList();
    out << m_attributes["soloPlayers"].toStringList();
    out << m_attributes["comments"].toString();
#else
    out << m_attributes;
#endif
    out << &m_program;
    out << qint32(m_attachments.length());
    for (const Attachment* a : m_attachments)
    {
        out << a->type();
        out << a;
    }
}

QString Song::attributeDisplay(int i) const
{
    return attributeDisplay(m_attributes.keys()[i]);
}

QString Song::attributeDisplay(const QString &key) const
{
    if (key == "label")
    {
        return Song::labelNames()[static_cast<int>(m_attributes["label"].value<Label>())];
    }
    if (key == "state")
    {
        return Song::stateNames()[static_cast<int>(m_attributes["state"].value<State>())];
    }
    if (key == "key")
    {
        return m_attributes["key"].value<Chord>().key();
    }
    if (key == "duration")
    {
        return m_attributes["duration"].toTime().toString("mm:ss");
    }
    if (m_attributes[key].canConvert(QVariant::StringList))
    {
        return m_attributes[key].toStringList().join(", ");
    }
    if (m_attributes[key].canConvert(QVariant::String))
    {
        return m_attributes[key].toString();
    }
    if (m_attributes[key].isNull())
    {
        return "";
    }
    Q_UNREACHABLE();
    return QString();
}

QStringList Song::labelNames()
{
    return QStringList({ tr(""), tr("Acoustic"), tr("Normal"), tr("A/N")});
}

QStringList Song::stateNames()
{
    return QStringList({ tr(""), tr("Inactive"), tr("Needs Practice"), tr("Works") });
}


QDataStream& operator <<(QDataStream& out, const Song::State& state)
{
    out << static_cast<qint32>(state);
    return out;
}

QDataStream& operator >>(QDataStream& in,        Song::State& state)
{
    qint32 fstate;
    in >> fstate;
    state = static_cast<Song::State>(fstate);
    return in;
}

QDataStream& operator <<(QDataStream& out, const Song::Label& label)
{
    out << static_cast<qint32>(label);
    return out;
}

QDataStream& operator >>(QDataStream& in,        Song::Label& label)
{
    qint32 flabel;
    in >> flabel;
    label = static_cast<Song::Label>(flabel);
    return in;
}
