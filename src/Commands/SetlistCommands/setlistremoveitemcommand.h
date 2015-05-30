#ifndef SETLISTREMOVEITEMCOMMAND_H
#define SETLISTREMOVEITEMCOMMAND_H

#include "setlistcommand.h"

class SetlistRemoveItemCommand : public SetlistCommand
{
public:
    SetlistRemoveItemCommand( Setlist* setlist, SetlistItem* item );
    ~SetlistRemoveItemCommand();
    void undo();
    void redo();

private:
    bool m_ownsItem = false;
    SetlistItem* const m_item;
    int m_index;
};

#endif // SETLISTREMOVEITEMCOMMAND_H
