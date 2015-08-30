#include "setlistremoveitemcommand.h"

SetlistRemoveItemCommand::SetlistRemoveItemCommand(Setlist *setlist, SetlistItem *setlistItem) :
    ModelCommand(setlist),
    ItemOwnerCommandDecorator(setlistItem),
    m_index( setlist->indexOf(setlistItem).row() )
{
    setText( CommandTranslator::tr("Remove Setlist Item") );
}


void SetlistRemoveItemCommand::redo()
{
    model()->removeItem( item() );
    takeOwnershipOfItem();
}

void SetlistRemoveItemCommand::undo()
{
    model()->insertItem( item(), m_index );
    releaseOwnershipOfItem();
}
