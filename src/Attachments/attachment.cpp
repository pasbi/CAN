#include "attachment.h"
#include "Database/SongDatabase/song.h"
#include <QBuffer>
#include "Database/SongDatabase/attachmentdatabase.h"


Attachment::Attachment() :
    DatabaseItem(nullptr),
    m_song(nullptr)
{
    addAttributeKey("name");
}

Attachment::~Attachment()
{
}

void Attachment::setSong(Song *song)
{
    // set song, but do never overwrite.
    Q_ASSERT( !m_song );
    setDatabase(song->attachmentDatabase());

    m_song = song;
    makeNameUnique();
}

void Attachment::setName(const QString &name)
{
    if (name != attribute("name").toString())
    {
        setAttribute("name", name);
        emit attachmentRenamed( name );
    }
}

void Attachment::makeNameUnique()
{
    Q_ASSERT( m_song );

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

QString Attachment::name() const
{
    return attribute("name").toString();
}












