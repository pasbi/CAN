#ifndef SONGDUPLICATEATTACHMENTCOMMAND_H
#define SONGDUPLICATEATTACHMENTCOMMAND_H

#include "songcommand.h"

class SongDuplicateAttachmentCommand : public SongCommand
{
public:
    SongDuplicateAttachmentCommand(Attachment *source);
    ~SongDuplicateAttachmentCommand();

    Attachment* attachment() const { return m_attachment; }

    void undo();
    void redo();

private:
    Attachment* m_attachment = 0;
    Attachment* m_source = 0;
    bool m_ownsAttachment = false;
};

#endif // SONGDUPLICATEATTACHMENTCOMMAND_H
