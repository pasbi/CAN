#include "fileattachmentcommandsethashcommand.h"
#include "FileIndex/fileindex.h"
#include "application.h"

FileAttachmentCommandSetHashCommand::FileAttachmentCommandSetHashCommand(IndexedFileAttachment *attachment, const QByteArray &newHash) :
    AttachmentCommand( attachment ),
    m_newHash( newHash )
{
    m_oldHash = attachment->hash();

    QString oldFilename = app().fileIndex().filename( m_oldHash );
    QString newFilename = app().fileIndex().filename( m_newHash );
    setText( QString(CommandTranslator::tr("Set file %1 -> %2").arg( oldFilename, newFilename )) );
}

void FileAttachmentCommandSetHashCommand::undo()
{
    attachment<IndexedFileAttachment>()->setHash( m_oldHash );
}

void FileAttachmentCommandSetHashCommand::redo()
{
    attachment<IndexedFileAttachment>()->setHash( m_newHash );
}
