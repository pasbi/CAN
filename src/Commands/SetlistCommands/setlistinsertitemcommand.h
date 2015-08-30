#ifndef SETLISTINSERTITEMCOMMAND_H
#define SETLISTINSERTITEMCOMMAND_H

#include "setlistcommand.h"

//TODO write templated abstraction for all insert-item-commands with m_ownsItem-flag
class SetlistInsertItemCommand : public SetlistCommand
{
public:
    SetlistInsertItemCommand(Setlist* setlist, SetlistItem* item, int position = -1);
    ~SetlistInsertItemCommand();
    void undo();
    void redo();

private:
    bool m_ownsItem = false;
    SetlistItem* const m_item;
    const int m_position;

};

#endif // SETLISTINSERTITEMCOMMAND_H
