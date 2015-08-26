#include "setlistmoverowscommand.h"


SetlistMoveRowsCommand::SetlistMoveRowsCommand(Setlist *setlist, QList<DatabaseMimeData<SetlistItem>::IndexedItem> sortedSource, int row) :
    SetlistCommand(setlist),
    m_sortedSource(sortedSource),
    m_row(row)
{
    setText(CommandTranslator::tr("Move Items"));
}

void SetlistMoveRowsCommand::undo()
{
    if (setlist())
    {
        setlist()->beginResetModel();
        setlist()->m_items = m_recentStatus;
        setlist()->endResetModel();
    }
}

void SetlistMoveRowsCommand::redo()
{
    if (setlist())
    {
        m_recentStatus = setlist()->m_items;
        setlist()->beginResetModel();
        QList<SetlistItem*> items;
        int n = m_sortedSource.length();
        int insertPos = m_row;
        for (int i = n-1; i >= 0; --i)
        {
            int index = m_sortedSource[i].index;
            items.prepend( setlist()->m_items.takeAt( index ) );
            if (index < insertPos)
            {
                insertPos--;
            }
        }

        for (int i = 0; i < n; ++i)
        {
            setlist()->m_items.insert(insertPos + i, items[i]);
        }
        setlist()->endResetModel();
    }
}


