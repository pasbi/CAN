#ifndef EVENTDATABASENEWEVENTCOMMAND_H
#define EVENTDATABASENEWEVENTCOMMAND_H

#include "Commands/modelcommand.h"
#include "Database/EventDatabase/event.h"
#include "Commands/itemownercommanddecorator.h"

class EventDatabaseNewEventCommand : public EventDatabaseCommand, private ItemOwnerCommandDecorator<Event>
{
public:
    EventDatabaseNewEventCommand(EventDatabase* database, Event *event, int row = -1);
    void undo();
    void redo();

private:
    const int m_row;
};

#endif // EVENTDATABASENEWEVENTCOMMAND_H
