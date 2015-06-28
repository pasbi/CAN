#include "eventdatabaseneweventcommand.h"
#include "global.h"

EventDatabaseNewEventCommand::EventDatabaseNewEventCommand( EventDatabase *database, Event* event ) :
    EventDatabaseCommand( database ),
    m_event( event )
{
    if ( m_event == NULL )
    {
        m_event = new Event( database,
                             QDateTime::currentDateTime(),
                             QDateTime::currentDateTime(),
                             Event::Rehearsal,
                             "" );
    }

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
    eventDatabase()->appendEvent( m_event );
    m_ownsEvent = false;
}

void EventDatabaseNewEventCommand::undo()
{
    assert( m_event );
    eventDatabase()->removeEvent( m_event );
    m_ownsEvent = true;
}
