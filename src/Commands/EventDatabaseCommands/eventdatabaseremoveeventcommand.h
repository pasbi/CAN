#ifndef EVENTDATABASEREMOVEEVENTCOMMAND_H
#define EVENTDATABASEREMOVEEVENTCOMMAND_H

#include "Commands/modelcommand.h"
#include "Commands/itemownercommanddecorator.h"

class EventDatabaseRemoveEventCommand : public EventDatabaseCommand, private ItemOwnerCommandDecorator<Event>
{
public:
    EventDatabaseRemoveEventCommand( EventDatabase* database, Event* event );

    void undo();
    void redo();

private:
    const int m_index;
};

#endif // EVENTDATABASEREMOVEEVENTCOMMAND_H
