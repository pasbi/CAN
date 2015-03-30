#ifndef EVENTDATABASENEWEVENTCOMMAND_H
#define EVENTDATABASENEWEVENTCOMMAND_H

#include "eventdatabasecommand.h"
#include "Database/EventDatabase/event.h"

class EventDatabaseNewEventCommand : public EventDatabaseCommand
{
public:
    EventDatabaseNewEventCommand( EventDatabase* database , Event *event = NULL );
    ~EventDatabaseNewEventCommand();
    void undo();
    void redo();

private:
    Event* m_event;
    bool m_ownsEvent = false;
};

#endif // EVENTDATABASENEWEVENTCOMMAND_H
