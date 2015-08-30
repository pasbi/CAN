#ifndef FILEATTACHMENTCOMMANDSETHASHCOMMAND_H
#define FILEATTACHMENTCOMMANDSETHASHCOMMAND_H

#include "Commands/modelcommand.h"
#include "Attachments/indexedfileattachment.h"

class FileAttachmentCommandSetHashCommand : public AttachmentCommand<IndexedFileAttachment>
{
public:
    FileAttachmentCommandSetHashCommand( IndexedFileAttachment* attachment, const QByteArray & newHash );

    void undo();
    void redo();

private:
    const QByteArray m_oldHash;
    const QByteArray m_newHash;
};

#endif // FILEATTACHMENTCOMMANDSETHASHCOMMAND_H
