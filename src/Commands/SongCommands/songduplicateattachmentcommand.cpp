#include "songduplicateattachmentcommand.h"

SongDuplicateAttachmentCommand::SongDuplicateAttachmentCommand(Attachment* source) :
    SongCommand(source->song()),
    m_source( source )
{
    assert( source->song() );
    setText( QString(CommandTranslator::tr("Duplicate attachment %1").arg( source->description())) );
}

SongDuplicateAttachmentCommand::~SongDuplicateAttachmentCommand()
{
    if (m_ownsAttachment)
    {
        delete m_attachment;
        m_attachment = 0;
    }
}

void SongDuplicateAttachmentCommand::redo()
{
    if (!m_attachment)
    {
        m_source->copy( m_attachment );
        m_attachment->setSong( song() );
        m_attachment->makeNameUnique();
    }

    song()->insertAttachment( m_attachment, song()->attachments().indexOf(m_source) + 1 );
    m_ownsAttachment = false;
}

void SongDuplicateAttachmentCommand::undo()
{
    assert( m_attachment );

    song()->removeAttachment( m_attachment );
    m_ownsAttachment = true;
}
