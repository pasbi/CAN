#include "setlistadditemcommand.h"

SetlistAddItemCommand::SetlistAddItemCommand(Setlist *setlist, SetlistItem *newItem) :
    SetlistCommand( setlist ),
    m_newItem( newItem )
{
}

SetlistAddItemCommand::~SetlistAddItemCommand()
{
    if (m_ownsItem)
    {
        delete m_newItem;
    }
}

void SetlistAddItemCommand::undo()
{
    setlist()->removeItem( m_newItem );
    m_ownsItem = true;
}

void SetlistAddItemCommand::redo()
{
    setlist()->appendItem( m_newItem );
    m_ownsItem = false;
}
