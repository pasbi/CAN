#ifndef SONG_H
#define SONG_H

#include "taggable.h"
#include <QList>
#include "Attachments/attachment.h"

class Song : public Taggable
{
public:
    Song();

private:
    QList<Attachment*> m_attachments;
};

#endif // SONG_H
