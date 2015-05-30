#ifndef SETLISTINSERTITEMCOMMAND_H
#define SETLISTINSERTITEMCOMMAND_H

#include "setlistcommand.h"

class SetlistInsertItemCommand : public SetlistCommand
{
public:
    SetlistInsertItemCommand(Setlist* setlist, int position, SetlistItem* item);
    ~SetlistInsertItemCommand();
    void undo();
    void redo();

private:
    const int m_position;
    SetlistItem* const m_item;
    bool m_ownsItem = false;


};

#endif // SETLISTINSERTITEMCOMMAND_H
