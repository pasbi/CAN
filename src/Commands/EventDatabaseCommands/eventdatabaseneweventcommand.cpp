#include "eventdatabaseneweventcommand.h"
#include "global.h"

EventDatabaseNewEventCommand::EventDatabaseNewEventCommand( EventDatabase *database, Event* event, int row ) :
    EventDatabaseCommand( database ),
    ItemOwnerCommandDecorator(event),
    m_row(row)
{
    setText( CommandTranslator::tr("New Event") );
}

void EventDatabaseNewEventCommand::redo()
{
    model()->insertEvent( item(), m_row );
    takeOwnershipOfItem();
}

void EventDatabaseNewEventCommand::undo()
{
    model()->removeEvent( item() );
    releaseOwnershipOfItem();
}
