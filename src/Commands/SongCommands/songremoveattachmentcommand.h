#ifndef SONGREMOVEATTACHMENTCOMMAND_H
#define SONGREMOVEATTACHMENTCOMMAND_H

#include "Commands/modelcommand.h"
#include "Commands/itemownercommanddecorator.h"

class SongRemoveAttachmentCommand : public ModelCommand<Song>, private ItemOwnerCommandDecorator<Attachment>
{
public:
    SongRemoveAttachmentCommand(Song *song, int i);

    void undo();
    void redo();

private:
    const int m_index;
};

#endif // SONGREMOVEATTACHMENTCOMMAND_H
