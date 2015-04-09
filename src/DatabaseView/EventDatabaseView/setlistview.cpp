#include "setlistview.h"
#include <QMimeData>
#include "global.h"
#include "application.h"
#include "project.h"
#include "Commands/SetlistCommands/setlistinsertitemcommand.h"
#include <QAction>
#include <QHeaderView>

SetlistView::SetlistView(QWidget *parent) :
    QTableView(parent)
{
    setAcceptDrops(true);
    setDropIndicatorShown( false );
    setDragDropMode( DragDrop );
    setSelectionMode( QAbstractItemView::ExtendedSelection );
    setDefaultDropAction( Qt::MoveAction );
    horizontalHeader()->hide();
    horizontalHeader()->setStretchLastSection( true );
    setHorizontalScrollMode( QTableView::ScrollPerPixel );
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

void SetlistView::paste( const QMimeData* mime )
{
    if (mime->hasFormat("CAN/Setlist/Item"))
    {
        app().project()->beginMacro(tr("Paste setlist items"));

        QDataStream stream( mime->data("CAN/Setlist/Item"));

        QList<SetlistItem*> items;
        stream >> items;
        for (SetlistItem* item : items)
        {
            app().project()->pushCommand( new SetlistInsertItemCommand( model(), model()->rowCount(), item ));
        }
        model()->notifyDataChanged( model()->index(model()->indexOf(items.first()), 0),
                                    model()->index(model()->indexOf(items.last()),  0) );

        app().project()->endMacro();
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

    Qt::DropAction dropaction = e->proposedAction();

    if (e->mimeData()->hasFormat("CAN/songs"))
    {
        // always copy this mimetype
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
        if ( dropaction == Qt::MoveAction )
        {
            app().project()->beginMacro(tr("Move setlist items"));
        }
        else
        {
            app().project()->beginMacro(tr("Copy setlist items"));
        }

        QDataStream stream( e->mimeData()->data("CAN/Setlist/Item"));

        QList<SetlistItem*> items;
        stream >> items;
        for (SetlistItem* item : items)
        {
            app().project()->pushCommand( new SetlistInsertItemCommand( model(), position, item ));
        }
        model()->notifyDataChanged( model()->index(model()->indexOf(items.first()), 0),
                                    model()->index(model()->indexOf(items.last()),  0) );
        if ( dropaction == Qt::CopyAction )
        {
            // copy
            e->accept();
        }
        else
        {
            // move, remove source items
            model()->removeDraggedItems();
            // e->accept() must not be called here since this makes the view to delete the dragged item
            // (which is exactly what we want. But the view deletes it without involving the remove-item-command)
        }

        app().project()->endMacro();
    }
}

void SetlistView::mousePressEvent(QMouseEvent *event)
{
    QTableView::mousePressEvent( event );
    emit clicked();
}

void SetlistView::setModel(Setlist *setlist)
{
     QTableView::setModel( setlist );
     connect(setlist, SIGNAL(dataChanged(QModelIndex,QModelIndex)), this, SLOT(updateMinimumHorizontalHeaderSize()));
     updateMinimumHorizontalHeaderSize();
}

void SetlistView::updateMinimumHorizontalHeaderSize()
{
    horizontalHeader()->setMinimumWidth( sizeHintForColumn( 0 ) );

    // shake the size to show or hide scrollbars that might have become visible or hidden
    resize( QSize( size().width() + 1, size().height() + 1) );
    resize( QSize( size().width() - 1, size().height() - 1) );
}
