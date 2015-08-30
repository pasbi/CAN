#ifndef SONGNEWATTACHMENTCOMMAND_H
#define SONGNEWATTACHMENTCOMMAND_H

#include "Commands/modelcommand.h"
#include "Commands/itemownercommanddecorator.h"

class SongNewAttachmentCommand : public ModelCommand<Song>, private ItemOwnerCommandDecorator<Attachment>
{
public:
    SongNewAttachmentCommand(Song* song, Attachment* attachment);

    void undo();
    void redo();
};

#endif // SONGNEWATTACHMENTCOMMAND_H
