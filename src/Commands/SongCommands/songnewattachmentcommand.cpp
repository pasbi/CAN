#include "songnewattachmentcommand.h"
#include "Database/SongDatabase/song.h"
#include "Attachments/attachment.h"

SongNewAttachmentCommand::SongNewAttachmentCommand(Song *song, Attachment *attachment) :
    ModelCommand<Song>(song),
    ItemOwnerCommandDecorator(attachment)
{
    attachment->setSong(song);
}

void SongNewAttachmentCommand::redo()
{
    model()->addAttachment( item() );
    releaseOwnershipOfItem();
}

void SongNewAttachmentCommand::undo()
{
    model()->removeAttachment( item() );
    takeOwnershipOfItem();
}
