#include "songtableviewcontainer.h"
#include "songtableview.h"

SongTableViewContainer::SongTableViewContainer(QWidget *parent) :
    DatabaseViewContainer(new SongTableView(), parent)
{
}
