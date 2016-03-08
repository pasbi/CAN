#include "song.h"
#include "songdatabase.h"
#include "global.h"
#include <QJsonDocument>
#include "Attachments/attachment.h"
#include "Database/EventDatabase/event.h"
#include "Database/EventDatabase/eventdatabase.h"

const QStringList Song::ATTRIBUTE_KEYS = {"title", "artist", "duration", "key", "label", "state", "singers", "soloPlayers", "comments", "creationDateTime"};

Song::Song(Database<Song> * database) :
    DatabaseItem(ATTRIBUTE_KEYS, database)
{
    setAttribute("creationDateTime", QDateTime::currentDateTime());
    setAttribute("label", NoLabel);
    setAttribute("state", NoState);
    setAttribute("key", QVariant::fromValue(Chord()));
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


void Song::deserialize(QDataStream &in)
{
    DatabaseItem::deserialize(in);

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

    out << &m_program;
    out << qint32(m_attachments.length());
    for (const Attachment* a : m_attachments)
    {
        out << a->type();
        out << a;
    }
}
QString Song::attributeDisplay(const QString &key) const
{
    QVariant attribute = Song::attribute(key);
    if (key == "label")
    {
        return Song::labelNames()[static_cast<int>(attribute.value<Label>())];
    }
    if (key == "state")
    {
        return Song::stateNames()[static_cast<int>(attribute.value<State>())];
    }
    if (key == "key")
    {
        return attribute.value<Chord>().key();
    }
    if (key == "duration")
    {
        return attribute.toTime().toString(preference<QString>("durationFormat"));
    }
    if (key == "creationDateTime")
    {
        return attribute.toDateTime().toString(preference<QString>("dateTimeFormat"));
    }
    if (attribute.canConvert(QVariant::StringList))
    {
        return attribute.toStringList().join(", ");
    }
    if (attribute.canConvert(QVariant::String))
    {
        return attribute.toString();
    }
    if (attribute.isNull())
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

QString Song::label() const
{
    return QString("%1 -- %2").arg(attributeDisplay("title"), attributeDisplay("artist"));
}

bool Song::canRemove() const
{
    // check if song is used in setlist
    for (Event* event: database()->project()->eventDatabase()->items())
    {
        if (event->needsSong(this))
        {
            return false;
        }
    }

    return true;
}

DEFINE_ENUM_STREAM_OPERATORS(Song::State)
DEFINE_ENUM_STREAM_OPERATORS(Song::Label)
