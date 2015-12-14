#include "songremoveattachmentcommand.h"
#include "Database/SongDatabase/song.h"
#include "Attachments/attachment.h"

SongRemoveAttachmentCommand::SongRemoveAttachmentCommand(Song* song, int i) :
    ModelCommand<Song>(song),
    ItemOwnerCommandDecorator(song->attachment( i )),
    m_index(i)
{
}

void SongRemoveAttachmentCommand::undo()
{
    releaseOwnershipOfItem();
    model()->insertAttachment( item(), m_index );
}

void SongRemoveAttachmentCommand::redo()
{
    takeOwnershipOfItem();
    item()->deactivate();
    model()->removeAttachment( item() );
}
