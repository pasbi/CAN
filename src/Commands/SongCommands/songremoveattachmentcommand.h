#ifndef SONGREMOVEATTACHMENTCOMMAND_H
#define SONGREMOVEATTACHMENTCOMMAND_H

#include "songcommand.h"

class SongRemoveAttachmentCommand : public SongCommand
{
public:
    SongRemoveAttachmentCommand(Song *song, int i);
    ~SongRemoveAttachmentCommand();

    void undo();
    void redo();

private:
    bool m_ownsAttachment = false;
    Attachment* m_attachment;
    int m_index;
};

#endif // SONGREMOVEATTACHMENTCOMMAND_H
