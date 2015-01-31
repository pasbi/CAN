#ifndef FILEATTACHMENTCOMMANDMODIFYFILENAMECOMMAND_H
#define FILEATTACHMENTCOMMANDMODIFYFILENAMECOMMAND_H

#include "Commands/command.h"
#include "Attachments/indexedfileattachment.h"

class FileAttachmentCommandModifyFilenameCommand : public Command
{
public:
    FileAttachmentCommandModifyFilenameCommand( IndexedFileAttachment* attachment, const QString & newFilename );

    void undo();
    void redo();

private:
    IndexedFileAttachment* m_attachment;
    QString m_oldFilename;
    QString m_newFilename;
};

#endif // FILEATTACHMENTCOMMANDMODIFYFILENAMECOMMAND_H
