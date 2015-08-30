#include "attachmentrenamecommand.h"

AttachmentRenameCommand::AttachmentRenameCommand(Attachment *attachment, const QString &newName) :
    AttachmentCommand( attachment ),
    m_newName( newName ),
    m_oldName( attachment->name() )
{
    setText( CommandTranslator::tr("rename attachment") );
}

void AttachmentRenameCommand::redo()
{
    model()->setName( m_newName );
}

void AttachmentRenameCommand::undo()
{
    model()->setName( m_oldName );
}
