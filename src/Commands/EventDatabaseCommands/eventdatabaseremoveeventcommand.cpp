#include "eventdatabaseremoveeventcommand.h"

EventDatabaseRemoveEventCommand::EventDatabaseRemoveEventCommand(EventDatabase *database, Event *event) :
    EventDatabaseCommand( database ),
    ItemOwnerCommandDecorator(event),
    m_index(database->indexOfEvent(event).row())
{
    assert( event );
    setText( CommandTranslator::tr("Delete Event") );
}

void EventDatabaseRemoveEventCommand::undo()
{
    takeOwnershipOfItem();
    model()->insertEvent( item(), m_index );
}

void EventDatabaseRemoveEventCommand::redo()
{
    releaseOwnershipOfItem();
    model()->removeEvent( item() );
}
