#ifndef SONGTABLE_H
#define SONGTABLE_H

#include "DatabaseView/databaseview.h"

class Song;
class SongTableView : public DatabaseView<Song>
{
    Q_OBJECT
public:
    explicit SongTableView(QWidget *parent = 0);
};

#endif // SONGTABLE_H
