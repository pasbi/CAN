#ifndef EVENTDATABASENEWEVENTCOMMAND_H
#define EVENTDATABASENEWEVENTCOMMAND_H

#include "eventdatabasecommand.h"
#include "Database/EventDatabase/event.h"

class EventDatabaseNewEventCommand : public EventDatabaseCommand
{
public:
    EventDatabaseNewEventCommand(EventDatabase* database, Event *event, int row = -1);
    ~EventDatabaseNewEventCommand();
    void undo();
    void redo();

private:
    Event* m_event;
    bool m_ownsEvent = false;
    const int m_row;
};

#endif // EVENTDATABASENEWEVENTCOMMAND_H
