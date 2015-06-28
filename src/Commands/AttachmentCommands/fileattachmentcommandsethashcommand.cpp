#include "fileattachmentcommandsethashcommand.h"
#include "FileIndex/fileindex.h"
#include "application.h"

FileAttachmentCommandSetHashCommand::FileAttachmentCommandSetHashCommand(IndexedFileAttachment *attachment, const QByteArray &newHash) :
    AttachmentCommand( attachment ),
    m_newHash( newHash )
{
    m_oldHash = attachment->hash();

    setText( CommandTranslator::tr("set file") );
}

void FileAttachmentCommandSetHashCommand::undo()
{
    attachment<IndexedFileAttachment>()->setHash( m_oldHash );
}

void FileAttachmentCommandSetHashCommand::redo()
{
    attachment<IndexedFileAttachment>()->setHash( m_newHash );
}
