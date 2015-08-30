#include "songremoveattachmentcommand.h"

SongRemoveAttachmentCommand::SongRemoveAttachmentCommand(Song* song, int i) :
    ModelCommand<Song>(song),
    ItemOwnerCommandDecorator(song->attachment( i )),
    m_index(i)
{
    setText( CommandTranslator::tr("Remove Attachment") );
}

void SongRemoveAttachmentCommand::undo()
{
    releaseOwnershipOfItem();
    model()->insertAttachment( item(), m_index );
}

void SongRemoveAttachmentCommand::redo()
{
    takeOwnershipOfItem();
    model()->removeAttachment( item() );
}
