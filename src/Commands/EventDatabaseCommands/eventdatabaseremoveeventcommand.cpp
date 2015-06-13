#include "eventdatabaseremoveeventcommand.h"

EventDatabaseRemoveEventCommand::EventDatabaseRemoveEventCommand(EventDatabase *database, Event *event) :
    EventDatabaseCommand( database ),
    m_ownsEvent( false ),
    m_event( event )
{
    assert( event );
    setText( QString(CommandTranslator::tr("Delete event %1").arg( event->description() )) );
}

EventDatabaseRemoveEventCommand::~EventDatabaseRemoveEventCommand()
{
    if (m_ownsEvent)
    {
        delete m_event;
        m_event = NULL;
    }
}

void EventDatabaseRemoveEventCommand::undo()
{
    m_ownsEvent = false;
    eventDatabase()->insertEvent( m_event, m_index );
}

void EventDatabaseRemoveEventCommand::redo()
{
    m_ownsEvent = true;
    m_index = eventDatabase()->removeEvent( m_event );
}
