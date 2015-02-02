#ifndef ATTACHMENTCOMMAND_H
#define ATTACHMENTCOMMAND_H

#include "Commands/command.h"
#include "Database/SongDatabase/song.h"
#include "Attachments/attachment.h"

class AttachmentCommand : public Command
{
public:
    AttachmentCommand(Attachment *attachment );

    template<typename T>
    T* attachment() const { return dynamic_cast<T*>( m_attachment ); }

private:
    Attachment* m_attachment;
};

#endif // ATTACHMENTCOMMAND_H
