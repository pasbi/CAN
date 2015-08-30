#ifndef SETLISTNEWITEMCOMMAND_H
#define SETLISTNEWITEMCOMMAND_H

#include "Commands/modelcommand.h"
#include "Commands/itemownercommanddecorator.h"

class SetlistNewItemCommand : public ModelCommand<Setlist>, private ItemOwnerCommandDecorator<SetlistItem>
{
public:
    SetlistNewItemCommand(Setlist* setlist, SetlistItem* setlistItem, int position = -1);
    void undo();
    void redo();

private:
    const int m_position;

};

#endif // SETLISTNEWITEMCOMMAND_H
