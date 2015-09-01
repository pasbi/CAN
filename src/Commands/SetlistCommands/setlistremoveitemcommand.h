#ifndef SETLISTREMOVEITEMCOMMAND_H
#define SETLISTREMOVEITEMCOMMAND_H

#include "Commands/modelcommand.h"
#include "Commands/itemownercommanddecorator.h"

class SetlistItem;
class Setlist;
class SetlistRemoveItemCommand : public ModelCommand<Setlist>, private ItemOwnerCommandDecorator<SetlistItem>
{
public:
    SetlistRemoveItemCommand( Setlist* setlist, SetlistItem* item );
    void undo();
    void redo();

private:
    const int m_index;
};

#endif // SETLISTREMOVEITEMCOMMAND_H
