#ifndef ATTACHMENTRENAMECOMMAND_H
#define ATTACHMENTRENAMECOMMAND_H

#include "Commands/modelcommand.h"

class Attachment;
class AttachmentRenameCommand : public ModelCommand<Attachment>
{
public:
    AttachmentRenameCommand(Attachment* attachment, const QString & newName);

    void redo();
    void undo();

private:
    const QString m_newName;
    const QString m_oldName;
};

#endif // ATTACHMENTRENAMECOMMAND_H
