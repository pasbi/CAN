#include "setlistview.h"
#include <QMimeData>
#include "application.h"
#include "project.h"
#include "Commands/SetlistCommands/setlistinsertitemcommand.h"
#include <QAction>
#include <QHeaderView>
#include <QClipboard>
#include "util.h"

const QString SetlistView::ITEMS_MIMEDATA_FORMAT = "CAN/Setlist/Item";

SetlistView::SetlistView(QWidget *parent) :
    QTableView(parent)
{
    setAcceptDrops(true);
    setDropIndicatorShown( false );
    setDragDropMode( DragDrop );
    setSelectionMode( QAbstractItemView::ExtendedSelection );
    setDefaultDropAction( Qt::MoveAction );
    setAlternatingRowColors( true );
    horizontalHeader()->hide();
    horizontalHeader()->setStretchLastSection( true );
    setHorizontalScrollMode( QTableView::ScrollPerPixel );

    setContextMenuPolicy( Qt::CustomContextMenu );
    connect(this, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(showContextMenu(QPoint)));

    initAction( actionNewSetlistItem,    this, tr("&New Item"),       tr("Insert new item"),        "Ctrl+N",   NULL, "" )
    initAction( actionDeleteSetlistItem, this, tr("&Remove Item"),    tr("Delete selected items"),  "Del",      NULL, "" )
    initAction( actionCopySetlistItem,   this, tr("&Copy Items"),     tr("Copy selected items"),    "Ctrl+C",   NULL, "" )
    initAction( actionPasteSetlistItem,  this, tr("&Paste Items"),    tr("Paste items"),            "Ctrl+V",   NULL, "" )
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
    if (mime->hasFormat( ITEMS_MIMEDATA_FORMAT ))
    {
        app().project()->beginMacro(tr("Paste setlist items"));

        QDataStream stream( mime->data( ITEMS_MIMEDATA_FORMAT ));

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
     if (setlist)
     {
         connect(setlist, SIGNAL(dataChanged(QModelIndex,QModelIndex)), this, SLOT(updateMinimumHorizontalHeaderSize()));
         updateMinimumHorizontalHeaderSize();
     }
}

void SetlistView::updateMinimumHorizontalHeaderSize()
{
    horizontalHeader()->setMinimumWidth( qMin(0, sizeHintForColumn( 0 )) );

    // shake the size to show or hide scrollbars that might have become visible or hidden
    resize( QSize( size().width() + 1, size().height() + 1) );
    resize( QSize( size().width() - 1, size().height() - 1) );
}

void SetlistView::showContextMenu(QPoint pos)
{
    QMenu* menu = new QMenu( this );
    setUpContextMenu(menu, pos);
    menu->popup(viewport()->mapToGlobal(pos));
    connect(menu, SIGNAL(aboutToHide()), menu, SLOT(deleteLater()));
}

void SetlistView::setUpContextMenu(QMenu *menu, QPoint pos)
{
    Q_UNUSED( pos );
    bool selectionIsEmpty = selectionModel()->selectedRows().isEmpty();

    menu->addActions( actions() );
    actions()[0]->setEnabled( !!model() );                           // new item
    actions()[1]->setEnabled( !!model() && !selectionIsEmpty );      // remove item
    actions()[2]->setEnabled( !!model() && !selectionIsEmpty );      // copy items
    actions()[3]->setEnabled( !!model() && app().clipboard()->mimeData()->formats().contains( SetlistView::ITEMS_MIMEDATA_FORMAT ) );     // paste items
}

#include "Commands/SetlistCommands/setlistadditemcommand.h"
void SetlistView::my_on_actionNewSetlistItem_triggered()
{
    if (model())
    {
        app().pushCommand( new SetlistAddItemCommand( model(), new SetlistItem() ) );
    }
}

#include "Commands/SetlistCommands/setlistremoveitemcommand.h"
void SetlistView::my_on_actionDeleteSetlistItem_triggered()
{
    QList<SetlistItem*> si = selectedItems();
    if (model() && !si.isEmpty())
    {
        app().project()->beginMacro( tr("Remove Setlist Items"));
        for (SetlistItem* i : si)
        {
            app().pushCommand( new SetlistRemoveItemCommand( model(), i ) );
        }
        app().project()->endMacro();
    }
}

void SetlistView::my_on_actionCopySetlistItem_triggered()
{
    QModelIndexList selection;
    if (selectionModel())
    {
        selection = selectionModel()->selectedRows();
    }
    app().clipboard()->setMimeData( model()->mimeData( selection ) );
}

void SetlistView::my_on_actionPasteSetlistItem_triggered()
{
    paste( app().clipboard()->mimeData() );
}

QList<SetlistItem*> SetlistView::selectedItems() const
{
    QList<SetlistItem*> items;
    if (model() && selectionModel())
    {
        for (const QModelIndex& index : selectionModel()->selectedRows())
        {
            items << model()->itemAt( index );
        }
        return items;
    }
    else
    {
        return QList<SetlistItem*>();
    }
}
