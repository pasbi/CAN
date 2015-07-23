#include "setlistadditemcommand.h"

SetlistAddItemCommand::SetlistAddItemCommand(Setlist *setlist, SetlistItem *newItem) :
    SetlistCommand( setlist ),
    m_newItem( newItem )
{
    setText( CommandTranslator::tr("add item to setlist") );
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
