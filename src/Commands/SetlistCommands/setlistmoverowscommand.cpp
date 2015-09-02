//#include "setlistmoverowscommand.h"
//#include "Database/EventDatabase/setlist.h"

//SetlistMoveRowsCommand::SetlistMoveRowsCommand(Setlist *setlist, QList<DatabaseMimeData<SetlistItem>::IndexedItem> sortedSource, int row) :
//    ModelCommand(setlist),
//    m_sortedSource(sortedSource),
//    m_recentStatus(model()->m_items),
//    m_row(row)
//{
//    setText(CommandTranslator::tr("Move Items"));
//}

//void SetlistMoveRowsCommand::undo()
//{
//    model()->beginResetModel();
//    model()->m_items = m_recentStatus;
//    model()->endResetModel();
//}

//void SetlistMoveRowsCommand::redo()
//{
//    model()->beginResetModel();
//    QList<SetlistItem*> items;
//    int n = m_sortedSource.length();
//    int insertPos = m_row;
//    for (int i = n-1; i >= 0; --i)
//    {
//        int index = m_sortedSource[i].index;
//        items.prepend( model()->m_items.takeAt( index ) );
//        if (index < insertPos)
//        {
//            insertPos--;
//        }
//    }

//    for (int i = 0; i < n; ++i)
//    {
//        model()->m_items.insert(insertPos + i, items[i]);
//    }
//    model()->endResetModel();
//}


