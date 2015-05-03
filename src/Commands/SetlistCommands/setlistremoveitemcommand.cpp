#include "setlistremoveitemcommand.h"

SetlistRemoveItemCommand::SetlistRemoveItemCommand(Setlist *setlist, SetlistItem *item) :
    SetlistCommand(setlist),
    m_item(item)
{
}

SetlistRemoveItemCommand::~SetlistRemoveItemCommand()
{
    if (m_ownsItem)
    {
        delete m_item;
    }
}

void SetlistRemoveItemCommand::redo()
{
    m_index = setlist()->indexOf( m_item );
    setlist()->removeItem( m_item );
    m_ownsItem = true;
}

void SetlistRemoveItemCommand::undo()
{
    setlist()->insertItem( m_index, m_item );
    m_ownsItem = false;
}
