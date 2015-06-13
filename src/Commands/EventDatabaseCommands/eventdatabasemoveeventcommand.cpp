#include "eventdatabasemoveeventcommand.h"

EventDatabaseMoveEventCommand::EventDatabaseMoveEventCommand(EventDatabase *model, int sourceRow, int targetRow) :
    EventDatabaseCommand( model ),
    m_sourceRow( sourceRow ),
    m_targetRow( targetRow )
{
    setText( QString(CommandTranslator::tr("Move event %1 -> %2").arg( m_sourceRow, m_targetRow )) );
}

void EventDatabaseMoveEventCommand::undo()
{
    eventDatabase()->moveRow( m_targetRow, m_sourceRow );
}

void EventDatabaseMoveEventCommand::redo()
{
    eventDatabase()->moveRow( m_sourceRow, m_targetRow );
}
