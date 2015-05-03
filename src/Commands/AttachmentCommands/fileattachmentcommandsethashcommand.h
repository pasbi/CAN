#ifndef FILEATTACHMENTCOMMANDSETHASHCOMMAND_H
#define FILEATTACHMENTCOMMANDSETHASHCOMMAND_H

#include "attachmentcommand.h"
#include "Attachments/indexedfileattachment.h"

class FileAttachmentCommandSetHashCommand : public AttachmentCommand
{
public:
    FileAttachmentCommandSetHashCommand( IndexedFileAttachment* attachment, const QByteArray & newHash );

    void undo();
    void redo();

private:
    QByteArray m_oldHash;
    QByteArray m_newHash;
};

#endif // FILEATTACHMENTCOMMANDSETHASHCOMMAND_H
