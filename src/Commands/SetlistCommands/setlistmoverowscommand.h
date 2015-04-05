#ifndef SETLISTMOVEROWSCOMMAND_H
#define SETLISTMOVEROWSCOMMAND_H

#include "setlistcommand.h"

class SetlistMoveRowsCommand : public SetlistCommand
{
public:
    SetlistMoveRowsCommand( Setlist* setlist, const QModelIndex& index, int direction );

    void undo();
    void redo();

private:
    QModelIndex m_index;
    int m_direction;
};

#endif // SETLISTMOVEROWSCOMMAND_H
