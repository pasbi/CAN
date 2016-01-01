#include "fileattachmentcommandsethashcommand.h"
#include "FileIndex/fileindex.h"
#include "Attachments/indexedfileattachment.h"

FileAttachmentCommandSetHashCommand::FileAttachmentCommandSetHashCommand(IndexedFileAttachment *attachment, const QByteArray &newHash) :
    ModelCommand( attachment ),
    m_oldHash( attachment->hash() ),
    m_newHash( newHash )
{
}

void FileAttachmentCommandSetHashCommand::undo()
{
    model()->setHash( m_oldHash );
}

void FileAttachmentCommandSetHashCommand::redo()
{
    model()->setHash( m_newHash );
}
