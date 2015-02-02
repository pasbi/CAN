#ifndef FILEATTACHMENTCOMMANDMODIFYFILENAMECOMMAND_H
#define FILEATTACHMENTCOMMANDMODIFYFILENAMECOMMAND_H

#include "attachmentcommand.h"
#include "Attachments/indexedfileattachment.h"

class FileAttachmentCommandModifyFilenameCommand : public AttachmentCommand
{
public:
    FileAttachmentCommandModifyFilenameCommand( IndexedFileAttachment* attachment, const QString & newFilename ) __attribute__ ((deprecated));

    void undo();
    void redo();

private:
    QString m_oldFilename;
    QString m_newFilename;
};

#endif // FILEATTACHMENTCOMMANDMODIFYFILENAMECOMMAND_H
