#ifndef SONGTABLEVIEWCONTAINER_H
#define SONGTABLEVIEWCONTAINER_H

#include <QWidget>
#include "DatabaseView/databaseviewcontainer.h"

class Song;
class SongTableViewContainer : public DatabaseViewContainer<Song>
{
    Q_OBJECT
public:
    explicit SongTableViewContainer(QWidget *parent = 0);
};

#endif // SONGTABLEVIEWCONTAINER_H
