#include "setlistview.h"
#include <QMimeData>
#include "global.h"
#include "application.h"
#include "project.h"
#include "Commands/SetlistCommands/setlistinsertitemcommand.h"

SetlistView::SetlistView(QWidget *parent) :
    QListView(parent)
{
    setAcceptDrops(true);
    setDropIndicatorShown(true);
    setDragDropMode( DragDrop );
    setSelectionMode( QAbstractItemView::ExtendedSelection );
}

bool acceptMimeData( const QMimeData* data )
{
    // CAN/song from SongDatabase, CAN/Setlist/Item from internal move/copy
    return data->hasFormat("CAN/songs") || data->hasFormat("CAN/Setlist/Item");
}

void SetlistView::dragEnterEvent(QDragEnterEvent *event)
{
    if ( model() && acceptMimeData(event->mimeData()) )
    {
        event->acceptProposedAction();
    }
}

void SetlistView::dragMoveEvent(QDragMoveEvent *e)
{
    if (model() && acceptMimeData(e->mimeData()))
    {
        e->acceptProposedAction();
    }
}



void SetlistView::dropEvent(QDropEvent *e)
{
    if (!model())
    {
        return;
    }

    int position = 0;
    int n = model()->rowCount(QModelIndex());
    QModelIndex i = indexAt(e->pos());
    if (i.isValid())
    {
        position = i.row();
    }
    else
    {
        position = n;
    }


    if (e->mimeData()->hasFormat("CAN/songs"))
    {
        QList<qintptr> ptrs;
        QDataStream stream( e->mimeData()->data("CAN/songs") );
        stream >> ptrs;
        if (!ptrs.isEmpty())
        {
            QList<SetlistItem*> newItems;
            app().project()->beginMacro(tr("Drop Songs"));
            for (qintptr ptr : ptrs)
            {
                Song* song = (Song*)( ptr );
                newItems << new SetlistItem( song );
                app().project()->pushCommand( new SetlistInsertItemCommand( model(), position, newItems.last() ) );
            }

            model()->notifyDataChanged( model()->index(model()->indexOf(newItems.first()), 0),
                                        model()->index(model()->indexOf(newItems.last()),  0) );
            app().project()->endMacro();
        }
        e->accept();
    }
    else if (e->mimeData()->hasFormat("CAN/Setlist/Item"))
    {
        QDataStream stream( e->mimeData()->data("CAN/Setlist/Item"));

        QList<SetlistItem*> items;
        stream >> items;
        for (SetlistItem* item : items)
        {
            app().project()->pushCommand( new SetlistInsertItemCommand( model(), position, item ));
        }
        model()->notifyDataChanged( model()->index(model()->indexOf(items.first()), 0),
                                    model()->index(model()->indexOf(items.last()),  0) );
    }



}
