#include "eventdatabaseneweventcommand.h"
#include "global.h"

EventDatabaseNewEventCommand::EventDatabaseNewEventCommand( EventDatabase *database, Event* event, int row ) :
    EventDatabaseCommand( database ),
    m_event( event ),
    m_row(row)
{
    setText( CommandTranslator::tr("new event") );
}

EventDatabaseNewEventCommand::~EventDatabaseNewEventCommand()
{
    if (m_ownsEvent)
    {
        delete m_event;
    }
}

void EventDatabaseNewEventCommand::redo()
{
    if (m_row < 0)
    {
        eventDatabase()->appendEvent( m_event );
    }
    else
    {
        eventDatabase()->insertEvent( m_event, m_row );
    }
    m_ownsEvent = false;
}

void EventDatabaseNewEventCommand::undo()
{
    assert( m_event );
    eventDatabase()->removeEvent( m_event );
    m_ownsEvent = true;
}
