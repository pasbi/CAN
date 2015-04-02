#ifndef SETLISTADDITEMCOMMAND_H
#define SETLISTADDITEMCOMMAND_H

#include "setlistcommand.h"

class SetlistAddItemCommand : public SetlistCommand
{
public:
    SetlistAddItemCommand( Setlist* setlist, SetlistItem* newItem );
    ~SetlistAddItemCommand();

    void undo();
    void redo();

private:
    int m_index;
    bool m_ownsItem = false;
    SetlistItem* const m_newItem;
};

#endif // SETLISTADDITEMCOMMAND_H
