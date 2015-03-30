#ifndef EVENTDATABASECOMMAND_H
#define EVENTDATABASECOMMAND_H

#include "../command.h"
#include "Database/EventDatabase/eventdatabase.h"

class EventDatabaseCommand : public Command
{
public:
    EventDatabaseCommand( EventDatabase* eventDatabase );
    EventDatabase* eventDatabase() const { return m_eventDatabase; }

private:
    EventDatabase* m_eventDatabase;
};

#endif // EVENTDATABASECOMMAND_H
