#include "setlistmoverowscommand.h"

SetlistMoveRowsCommand::SetlistMoveRowsCommand(Setlist *setlist, const QModelIndex &index, int direction) :
    SetlistCommand( setlist ),
    m_index( index ),
    m_direction( direction )
{
}

void SetlistMoveRowsCommand::undo()
{

}

void SetlistMoveRowsCommand::redo()
{

}
