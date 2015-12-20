#include "song.h"
#include "songdatabase.h"
#include "global.h"
#include <QJsonDocument>
#include "Attachments/attachment.h"


const QStringList Song::LABEL_NAMES = QStringList({ Song::tr(""), Song::tr("Acoustic"), Song::tr("Normal") });
const QStringList Song::STATE_NAMES = QStringList({ Song::tr(""), Song::tr("Inactive"), Song::tr("Needs Practice"), Song::tr("Works") });

Song::Song(Database<Song> * database) :
    DatabaseItem(database),
    m_creationDateTime(QDateTime::currentDateTime()),
    m_label(NoLabel),
    m_state(NoState),
    m_key(-1)
{
    connect( this, SIGNAL(attachmentAdded(int)),   database, SIGNAL(attachmentAdded(int)  ));
    connect( this, SIGNAL(attachmentRemoved(int)), database, SIGNAL(attachmentRemoved(int)));
    connect( this, SIGNAL(attachmentRenamed(int, QString)), database, SIGNAL(attachmentRenamed(int,QString)));
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

void Song::setTitle(const QString& title)
{
    if (title != m_title)
    {
        m_title = title;
        emit attributeChanged();
    }
}

void Song::setArtist(const QString& artist)
{
    if (artist != m_artist)
    {
        m_artist = artist;
        emit attributeChanged();
    }
}

void Song::setDuration(const QTime& duration)
{
    if (duration != m_duration)
    {
        m_duration = duration;
        emit attributeChanged();
    }
}

void Song::setlabel(Label label)
{
    m_label = label;
}

void Song::setState(State state)
{
    m_state = state;
}

void Song::setSingers(QList<int> singers)
{
    m_singers = singers;
}

void Song::setSoloPlayers(QList<int> soloPlayers)
{
    m_soloPlayers = soloPlayers;
}

void Song::setComments(const QString& comments)
{
    m_comments = comments;
}

void Song::setKey(int key)
{
    m_key = key;
}

QStringList Song::textAttributes() const
{
    return QStringList( { artist(), title() } );
}

void Song::deserialize(QDataStream &in)
{
    DatabaseItem::deserialize(in);
    qint32 n, state, label;
    in >> m_title >> m_artist >> m_creationDateTime >> m_duration;
    in >> state >> label >> m_key >> m_singers >> m_soloPlayers >> m_comments;
    m_state = static_cast<State>(state);
    m_label = static_cast<Label>(label);
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
    out << m_title << m_artist << m_creationDateTime << m_duration;
    out << static_cast<qint32>(m_state) << static_cast<qint32>(m_label);
    out << m_key << m_singers << m_soloPlayers << m_comments;
    out << &m_program;
    out << static_cast<qint32>(m_attachments.length());
    for (const Attachment* a : m_attachments)
    {
        out << a->type();
        out << a;
    }
}


