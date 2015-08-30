#include "fileattachmentcommandsethashcommand.h"
#include "FileIndex/fileindex.h"
#include "application.h"

FileAttachmentCommandSetHashCommand::FileAttachmentCommandSetHashCommand(IndexedFileAttachment *attachment, const QByteArray &newHash) :
    AttachmentCommand( attachment ),
    m_oldHash( attachment->hash() ),
    m_newHash( newHash )
{
    setText( CommandTranslator::tr("Set File") );
}

void FileAttachmentCommandSetHashCommand::undo()
{
    model()->setHash( m_oldHash );
}

void FileAttachmentCommandSetHashCommand::redo()
{
    model()->setHash( m_newHash );
}
