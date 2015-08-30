#ifndef SETLISTMOVEROWSCOMMAND_H
#define SETLISTMOVEROWSCOMMAND_H

#include "Commands/modelcommand.h"
#include <QList>
#include "Database/databasemimedata.h"

class SetlistMoveRowsCommand : public ModelCommand<Setlist>
{
public:
    SetlistMoveRowsCommand(Setlist* setlist, QList<DatabaseMimeData<SetlistItem>::IndexedItem> sortedSource, int row);

    void undo();
    void redo();

private:
    const QList<DatabaseMimeData<SetlistItem>::IndexedItem> m_sortedSource;
    const QList<SetlistItem*> m_recentStatus;
    const int m_row;
};


#endif // SETLISTMOVEROWSCOMMAND_H
