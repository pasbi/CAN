#include "songaddattachmentcommand.h"

SongAddAttachmentCommand::SongAddAttachmentCommand(Song *song, Attachment *attachment) :
    SongCommand(song),
    m_attachment(attachment)
{
    setText( CommandTranslator::tr("add song") );
    m_attachment->setSong(song);
}

SongAddAttachmentCommand::~SongAddAttachmentCommand()
{
    if (m_ownsAttachment)
    {
        delete m_attachment;
        m_attachment = 0;
    }
}

void SongAddAttachmentCommand::redo()
{
    song()->addAttachment( m_attachment );
    m_ownsAttachment = false;
}

void SongAddAttachmentCommand::undo()
{
    assert( m_attachment );

    song()->removeAttachment( m_attachment );
    m_ownsAttachment = true;
}
