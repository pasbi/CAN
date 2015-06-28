#include "setlistmoverowscommand.h"

SetlistMoveRowsCommand::SetlistMoveRowsCommand(Setlist *setlist, const QModelIndex &index, int direction) :
    SetlistCommand( setlist )
{
    m_from = index.row();
    m_to = m_from + direction;
    setText( CommandTranslator::tr("move items in setlist") );
}

void SetlistMoveRowsCommand::moveRow( int from, int to )
{
    Setlist* sl = setlist();
    if (sl)
    {
        sl->moveItem( from, to );
    }
}

void SetlistMoveRowsCommand::undo()
{
    moveRow( m_to, m_from );
}

void SetlistMoveRowsCommand::redo()
{
    moveRow( m_from, m_to );
}
