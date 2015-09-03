#include "eventtableviewcontainer.h"
#include "eventtableview.h"

EventTableViewContainer::EventTableViewContainer(QWidget *parent) :
    DatabaseViewContainer(new EventTableView(), parent)
{
}
