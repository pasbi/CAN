#ifndef SONGLIST_H
#define SONGLIST_H

#include <QList>
#include "Database/SongDatabase/song.h"

class SongList
{
public:
    SongList();

private:
    QList<Song*> m_songs;
};

#endif // SONGLIST_H
