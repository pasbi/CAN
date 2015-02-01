#ifndef ATTACHMENTRENAMECOMMAND_H
#define ATTACHMENTRENAMECOMMAND_H

#include "attachmentcommand.h"
#include "Attachments/attachment.h"

class AttachmentRenameCommand : public AttachmentCommand
{
public:
    AttachmentRenameCommand(Attachment* attachment, const QString & newName) __attribute__ ((deprecated));

    void redo();
    void undo();

private:
    Attachment* m_attachment;
    QString m_newName;
    QString m_oldName;
};

#endif // ATTACHMENTRENAMECOMMAND_H
