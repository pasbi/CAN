#include "songremoveattachmentcommand.h"

SongRemoveAttachmentCommand::SongRemoveAttachmentCommand(Song* song, int i) :
    SongCommand(song),
    m_index(i)
{
    m_attachment = song->attachment( m_index );
    setText( CommandTranslator::tr("remove attachment") );
}

SongRemoveAttachmentCommand::~SongRemoveAttachmentCommand()
{
    if (m_ownsAttachment)
    {
        delete m_attachment;
        m_attachment = 0;
    }
}

void SongRemoveAttachmentCommand::undo()
{
    m_ownsAttachment = false;
    song()->insertAttachment( m_attachment, m_index );
}

void SongRemoveAttachmentCommand::redo()
{
    m_ownsAttachment = true;
    m_index = song()->removeAttachment( m_attachment );
}
