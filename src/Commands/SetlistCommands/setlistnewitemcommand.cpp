#include "setlistnewitemcommand.h"

SetlistNewItemCommand::SetlistNewItemCommand(Setlist *setlist, SetlistItem *setlistItem, int position) :
    ModelCommand( setlist ),
    ItemOwnerCommandDecorator(setlistItem),
    m_position(position)
{
    setText( CommandTranslator::tr("Insert Setlist Item") );
}

void SetlistNewItemCommand::undo()
{
    model()->removeItem( item() );
    takeOwnershipOfItem();
}

void SetlistNewItemCommand::redo()
{
    model()->insertItem( item(), m_position );
    releaseOwnershipOfItem();
}
