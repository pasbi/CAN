#include "setlistinsertitemcommand.h"

SetlistInsertItemCommand::SetlistInsertItemCommand(Setlist *setlist, SetlistItem *item, int position) :
    SetlistCommand( setlist ),
    m_item(item),
    m_position(position)
{
    setText( CommandTranslator::tr("Insert Setlist item") );
}

SetlistInsertItemCommand::~SetlistInsertItemCommand()
{
    if (m_ownsItem)
    {
        delete m_item;
    }
}

void SetlistInsertItemCommand::undo()
{
    setlist()->removeItem( m_item );
    m_ownsItem = true;
}

void SetlistInsertItemCommand::redo()
{
    setlist()->insertItem( m_item, m_position );
    m_ownsItem = false;
}
