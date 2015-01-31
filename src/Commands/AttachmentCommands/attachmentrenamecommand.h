#ifndef ATTACHMENTRENAMECOMMAND_H
#define ATTACHMENTRENAMECOMMAND_H

#include "Commands/command.h"
#include "Attachments/attachment.h"

class AttachmentRenameCommand : public Command
{
public:
    AttachmentRenameCommand(Attachment* attachment, const QString & newName);

    void redo();
    void undo();

private:
    Attachment* m_attachment;
    QString m_newName;
    QString m_oldName;
};

#endif // ATTACHMENTRENAMECOMMAND_H
