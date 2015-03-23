#include "fileattachmentcommandmodifyfilenamecommand.h"

FileAttachmentCommandModifyFilenameCommand::FileAttachmentCommandModifyFilenameCommand(IndexedFileAttachment *attachment, const QString &newFilename) :
    AttachmentCommand( attachment ),
    m_newFilename( newFilename )
{
    m_oldFilename = "";//attachment->/*filename*/();
}

void FileAttachmentCommandModifyFilenameCommand::undo()
{
//    attachment<IndexedFileAttachment>()->addAlternative( m_oldFilename );
}

void FileAttachmentCommandModifyFilenameCommand::redo()
{
//    attachment<IndexedFileAttachment>()->removeA( m_newFilename );
}
