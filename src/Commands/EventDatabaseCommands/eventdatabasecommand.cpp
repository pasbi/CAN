#include "eventdatabasecommand.h"

EventDatabaseCommand::EventDatabaseCommand(EventDatabase *eventDatabase) :
    m_eventDatabase( eventDatabase )
{
    m_type = Command::EventDatabaseRelated;
}
