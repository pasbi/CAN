#include "fileattachmentcommandmodifyfilenamecommand.h"

FileAttachmentCommandModifyFilenameCommand::FileAttachmentCommandModifyFilenameCommand(IndexedFileAttachment *attachment, const QString &newFilename) :
    m_attachment(attachment),
    m_newFilename( newFilename )
{
    m_oldFilename = m_attachment->filename();
}

void FileAttachmentCommandModifyFilenameCommand::undo()
{
    m_attachment->setFilename( m_oldFilename );
}

void FileAttachmentCommandModifyFilenameCommand::redo()
{
    m_attachment->setFilename( m_newFilename );
}
