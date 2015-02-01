#include "attachment.h"
#include "Database/SongDatabase/song.h"

Attachment::Attachment()
{
}

void Attachment::setSong(Song *song)
{
    // set song, but do never overwrite.
    assert( !m_song );

    m_song = song;
}

void Attachment::setName(const QString &name)
{
    m_name = name;
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
