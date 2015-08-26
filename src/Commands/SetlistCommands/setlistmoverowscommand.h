#ifndef SETLISTMOVEROWSCOMMAND_H
#define SETLISTMOVEROWSCOMMAND_H

#include "setlistcommand.h"
#include <QList>
#include "Database/databasemimedata.h"

class SetlistMoveRowsCommand : public SetlistCommand
{
public:
    SetlistMoveRowsCommand(Setlist* setlist, QList<DatabaseMimeData<SetlistItem>::IndexedItem> sortedSource, int row);

    void undo();
    void redo();

private:
    QList<DatabaseMimeData<SetlistItem>::IndexedItem> m_sortedSource;
    QList<SetlistItem*> m_recentStatus;
    int m_row;
};


#endif // SETLISTMOVEROWSCOMMAND_H
