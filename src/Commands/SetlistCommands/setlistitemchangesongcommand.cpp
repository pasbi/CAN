#include "setlistitemchangesongcommand.h"
#include "Database/database.h"
#include "Database/EventDatabase/setlistitem.h"

SetlistItemChangeSongCommand::SetlistItemChangeSongCommand(SetlistItem *setlistItem, const Song *newSong) :
    ModelCommand(setlistItem),
    m_newSong(newSong),
    m_oldSong( setlistItem->attribute("song").value<const Song*>() )
{
}

void SetlistItemChangeSongCommand::undo()
{
    model()->setSong(m_oldSong);
    model()->database()->notifiyDataChange( model() );
}

void SetlistItemChangeSongCommand::redo()
{
    model()->setSong(m_newSong);
    model()->database()->notifiyDataChange( model() );
}
