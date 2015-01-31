#include "attachment.h"

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
