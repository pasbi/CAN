#ifndef EVENTTABLEVIEWCONTAINER_H
#define EVENTTABLEVIEWCONTAINER_H

#include <QWidget>
#include "DatabaseView/databaseviewcontainer.h"

class Event;
class EventTableViewContainer : public DatabaseViewContainer<Event>
{
    Q_OBJECT
public:
    explicit EventTableViewContainer(QWidget *parent = 0);
};

#endif // EVENTTABLEVIEWCONTAINER_H
