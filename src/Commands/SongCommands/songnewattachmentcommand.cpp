#include "songnewattachmentcommand.h"
#include "Database/SongDatabase/song.h"

SongNewAttachmentCommand::SongNewAttachmentCommand(Song *song, Attachment *attachment) :
    ModelCommand<Song>(song),
    ItemOwnerCommandDecorator(attachment)
{
    setText( CommandTranslator::tr("New Song") );
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
