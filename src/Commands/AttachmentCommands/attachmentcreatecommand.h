#ifndef ATTACHMENTCREATECOMMAND_H
#define ATTACHMENTCREATECOMMAND_H

#include "attachmentcommand.h"

class AttachmentCreateCommand : public AttachmentCommand
{
public:
    AttachmentCreateCommand(Song* song , const QString &classname) __attribute__ ((deprecated));
    ~AttachmentCreateCommand();

    void undo();
    void redo();

private:
    QString m_classname;
    Attachment* m_attachment = NULL;
    bool m_ownsAttachment = false;

};

#endif // ATTACHMENTCREATECOMMAND_H
