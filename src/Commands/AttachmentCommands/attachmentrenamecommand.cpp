#include "attachmentrenamecommand.h"

AttachmentRenameCommand::AttachmentRenameCommand(Attachment *attachment, const QString &newName) :
    AttachmentCommand( attachment ),
    m_attachment( attachment ),
    m_newName( newName )
{
    m_oldName = m_attachment->name();
}

void AttachmentRenameCommand::redo()
{
    m_attachment->setName( m_newName );
}

void AttachmentRenameCommand::undo()
{
    m_attachment->setName( m_oldName );
}
