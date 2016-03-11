#include "attachment.h"
#include "Database/SongDatabase/song.h"
#include <QBuffer>
#include "Database/SongDatabase/attachmentdatabase.h"

const QStringList Attachment::ATTRIBUTE_KEYS = {};

Attachment::Attachment() :
    DatabaseItem(ATTRIBUTE_KEYS, nullptr),
    m_song(nullptr)
{

}

Attachment::~Attachment()
{
}

void Attachment::setSong(Song *song)
{
    // set song, but do never overwrite.
    assert( !m_song );
    setDatabase(song->attachmentDatabase());

    m_song = song;
    makeNameUnique();
}

void Attachment::setName(const QString &name)
{
    if (name != m_name)
    {
        m_name = name;
        emit attachmentRenamed( m_name );
    }
}

void Attachment::makeNameUnique()
{
    assert( m_song );

    const QStringList names = m_song->attachmentNames();

    QString newName = name();
    int i = 1;
    while ( names.contains( newName ) )
    {
        newName = QString("%1 %2").arg(name()).arg(i++);
    }

    setName( newName );
}

Attachment* Attachment::create(const QString& classname)
{
    Attachment* attachment = Creatable::create<Attachment>( classname );
    Q_ASSERT(attachment);
    return attachment;
}

QString Attachment::label() const
{
    return name();
}

void Attachment::paint(PDFCreator*)
{
    Q_UNIMPLEMENTED();
}

QString Attachment::attributeDisplay(const QString &key) const
{
    Q_UNUSED(key);
    return "";
}

void Attachment::serialize(QDataStream& out) const
{
    DatabaseItem::serialize(out);
    out << m_name;
}

void Attachment::deserialize(QDataStream& in)
{
    DatabaseItem::deserialize(in);
    in >> m_name;
}

QDataStream& operator<<(QDataStream& out, const Attachment* attachment)
{
    attachment->serialize(out);
    return out;
}

QDataStream& operator>>(QDataStream& in, Attachment* attachment)
{
    attachment->deserialize(in);
    return in;
}













