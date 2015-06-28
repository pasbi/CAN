#include "eventdatabasemoveeventcommand.h"

EventDatabaseMoveEventCommand::EventDatabaseMoveEventCommand(EventDatabase *model, int sourceRow, int targetRow) :
    EventDatabaseCommand( model ),
    m_sourceRow( sourceRow ),
    m_targetRow( targetRow )
{
    setText( CommandTranslator::tr("move event") );
}

void EventDatabaseMoveEventCommand::undo()
{
    eventDatabase()->moveRow( m_targetRow, m_sourceRow );
}

void EventDatabaseMoveEventCommand::redo()
{
    eventDatabase()->moveRow( m_sourceRow, m_targetRow );
}
