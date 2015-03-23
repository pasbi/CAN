#include "fileattachmentcommandsethashcommand.h"

FileAttachmentCommandSetHashCommand::FileAttachmentCommandSetHashCommand(IndexedFileAttachment *attachment, const QByteArray &newHash) :
    AttachmentCommand( attachment ),
    m_newHash( newHash )
{
    m_oldHash = attachment->hash();
}

void FileAttachmentCommandSetHashCommand::undo()
{
    attachment<IndexedFileAttachment>()->setHash( m_oldHash );
}

void FileAttachmentCommandSetHashCommand::redo()
{
    attachment<IndexedFileAttachment>()->setHash( m_newHash );
}
