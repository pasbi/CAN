#ifndef SONGTABLE_H
#define SONGTABLE_H

#include "DatabaseView/databaseview.h"

class Song;
class SongTableView : public DatabaseView<Song>
{
    Q_OBJECT
public:
    explicit SongTableView(QWidget *parent = 0);

    void setDetailedView(bool detailed);
};

#endif // SONGTABLE_H
