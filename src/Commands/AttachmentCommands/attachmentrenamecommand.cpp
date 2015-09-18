#include "attachmentrenamecommand.h"

#include "Attachments/attachment.h"

AttachmentRenameCommand::AttachmentRenameCommand(Attachment *attachment, const QString &newName) :
    ModelCommand( attachment ),
    m_newName( newName ),
    m_oldName( attachment->name() )
{
}

void AttachmentRenameCommand::redo()
{
    model()->setName( m_newName );
}

void AttachmentRenameCommand::undo()
{
    model()->setName( m_oldName );
}
