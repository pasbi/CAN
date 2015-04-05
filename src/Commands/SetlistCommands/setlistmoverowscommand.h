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
    int m_from;
    int m_to;

    void moveRow(int from, int to);
};

#endif // SETLISTMOVEROWSCOMMAND_H
