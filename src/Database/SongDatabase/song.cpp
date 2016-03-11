#include "song.h"
#include "songdatabase.h"
#include "global.h"
#include <QJsonDocument>
#include "Attachments/attachment.h"
#include "Database/EventDatabase/event.h"
#include "Database/EventDatabase/eventdatabase.h"
#include "attachmentdatabase.h"
#include "Project/project.h"

const QStringList Song::ATTRIBUTE_KEYS = {"title", "artist", "duration", "key", "label", "state", "singers", "soloPlayers", "comments", "creationDateTime"};

Song::Song(Database<Song> * database) :
    DatabaseItem(ATTRIBUTE_KEYS, database),
    m_attachmentDatabase(new AttachmentDatabase(this, database->project()))
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
    delete m_attachmentDatabase;
    m_attachmentDatabase = 0;
}

QStringList Song::attachmentNames() const
{
    QStringList akk;
    for (const Attachment* a : attachments())
    {
        akk << a->name();
    }
    return akk;
}


int Song::removeAttachment( Attachment* attachment )
{
    Q_ASSERT(m_attachmentDatabase->items().contains(attachment));
    int index = m_attachmentDatabase->items().indexOf( attachment );
    m_attachmentDatabase->removeItem(attachment);
    emit attachmentRemoved( index );
    return index;
}

void Song::addAttachment( Attachment* attachment )
{
    int index = attachments().length();
    insertAttachment( attachment, index );
}

void Song::insertAttachment(Attachment *attachment, int index)
{
    assert( attachment->song() == this );
    m_attachmentDatabase->insertItem( attachment, index );
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

QList<Attachment*> Song::attachments() const
{
    return m_attachmentDatabase->items();
}

Attachment* Song::attachment( int i ) const
{
    return m_attachmentDatabase->items()[i];
}

void Song::serialize(QDataStream &out) const
{
    DatabaseItem::serialize(out);
    out << &m_program;
    out << m_attachmentDatabase;
}

void Song::deserialize(QDataStream &in)
{
    DatabaseItem::deserialize(in);

    in >> &m_program;
    in >> m_attachmentDatabase;
    for (Attachment* a : m_attachmentDatabase->items())
    {
        connectAttachment(a);
    }
}

AttachmentDatabase* Song::attachmentDatabase() const
{
    return m_attachmentDatabase;
}

DEFINE_ENUM_STREAM_OPERATORS(Song::State)
DEFINE_ENUM_STREAM_OPERATORS(Song::Label)
