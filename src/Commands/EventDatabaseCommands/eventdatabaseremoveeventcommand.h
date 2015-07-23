#ifndef EVENTDATABASEREMOVEEVENTCOMMAND_H
#define EVENTDATABASEREMOVEEVENTCOMMAND_H

#include "eventdatabasecommand.h"

class EventDatabaseRemoveEventCommand : public EventDatabaseCommand
{
public:
    EventDatabaseRemoveEventCommand( EventDatabase* database, Event* event );
    ~EventDatabaseRemoveEventCommand();

    void undo();
    void redo();

private:
    bool m_ownsEvent;
    Event* m_event;
    int m_index;
};

#endif // EVENTDATABASEREMOVEEVENTCOMMAND_H
