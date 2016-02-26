#include "attachment.h"
#include "Database/SongDatabase/song.h"
#include <QBuffer>

Attachment::Attachment() :
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

Attachment* Attachment::create(const QString& classname, Song* song)
{
    Attachment* attachment = Creatable::create<Attachment>( classname );
    attachment->setSong( song );
    return attachment;
}

void Attachment::serialize(QDataStream &out) const
{
    PersistentObject::serialize(out);
    out << name();
}

void Attachment::deserialize(QDataStream &in)
{
    PersistentObject::deserialize(in);
    in >> m_name;
}

QString Attachment::description() const
{
    return name();
}

Attachment* Attachment::copy() const
{
    return copy(song());
}

Attachment* Attachment::copy(Song* song) const
{
    QBuffer buffer;
    buffer.open(QIODevice::ReadWrite);
    QDataStream stream(&buffer);
    stream << this;

    Attachment* copy = create(type(), song);
    stream >> copy;

    return copy;
}

void Attachment::paint(PDFCreator*)
{
    Q_UNIMPLEMENTED();
}



















