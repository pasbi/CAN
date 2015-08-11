#include "setlistview.h"
#include <QMimeData>
#include "application.h"
#include "Project/project.h"
#include "Commands/SetlistCommands/setlistinsertitemcommand.h"
#include <QAction>
#include <QHeaderView>
#include <QClipboard>
#include "util.h"
#include <QToolButton>
#include "Dialogs/chordpatternviewer.h"


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
    setHorizontalScrollMode( QTableView::ScrollPerPixel );

    setEditTriggers( QAbstractItemView::SelectedClicked );

    setContextMenuPolicy( Qt::CustomContextMenu );
    connect(this, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(showContextMenu(QPoint)));

    initAction( actionNewSetlistItem,    this, tr("&New Item"),       tr("Insert new item"),        "Ctrl+N",   NULL, "" )
    initAction( actionDeleteSetlistItem, this, tr("&Remove Item"),    tr("Delete selected items"),  "Del",      NULL, "" )
    initAction( actionCopySetlistItem,   this, tr("&Copy Items"),     tr("Copy selected items"),    "Ctrl+C",   NULL, "" )
    initAction( actionPasteSetlistItem,  this, tr("&Paste Items"),    tr("Paste items"),            "Ctrl+V",   NULL, "" )

}

SetlistView::~SetlistView()
{
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
            int i = 0;
            for (qintptr ptr : ptrs)
            {
                Song* song = (Song*)( ptr );
                newItems << new SetlistItem( song );
                app().project()->pushCommand( new SetlistInsertItemCommand( model(), position + i++, newItems.last() ) );
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
    if (model())
    {
        disconnect( model(), SIGNAL(rowsInserted(QModelIndex,int,int)),                this, SLOT(updateCellWidgets()) );
        disconnect( model(), SIGNAL(rowsMoved(QModelIndex,int,int,QModelIndex,int)),   this, SLOT(updateCellWidgets()) );
        disconnect( model(), SIGNAL(rowsRemoved(QModelIndex,int,int)),                 this, SLOT(updateCellWidgets()) );
    }
    QTableView::setModel( setlist );
    if (setlist)
    {
        connect( setlist, SIGNAL(rowsInserted(QModelIndex,int,int)),                this, SLOT(updateCellWidgets()) );
        connect( setlist, SIGNAL(rowsMoved(QModelIndex,int,int,QModelIndex,int)),   this, SLOT(updateCellWidgets()) );
        connect( setlist, SIGNAL(rowsRemoved(QModelIndex,int,int)),                 this, SLOT(updateCellWidgets()) );
        updateCellWidgets();
        horizontalHeader()->setSectionResizeMode( 0, QHeaderView::Stretch );
        horizontalHeader()->setSectionResizeMode( 1, QHeaderView::Fixed );
        horizontalHeader()->resizeSection( 1, 45 );
    }
}

Setlist* SetlistView::model() const
{
    return qobject_assert_cast<Setlist*>( QTableView::model() );
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

bool SetlistView::includeAttachment( const Attachment* attachment )
{
    // if attachment is of wrong type, do not include it.
    if ( attachment->type() != ChordPatternAttachment::TYPE )
    {
        return false;
    }

    // if attachment has no tags, include it.
    else if ( attachment->tags().isEmpty() )
    {
        return true;
    }

    // if no filter tag is set, return true.
    else if ( m_filterTag.isEmpty() )
    {
        return true;
    }

    // if attachment has filter tag, include it.
    else if ( attachment->tags().contains( m_filterTag ) )
    {
        return true;
    }

    // otherwise, return false
    return false;
}

void SetlistView::updateCellWidgets()
{
    if (!model())
    {
        return;
    }

    for (int i = 0; i < model()->rowCount(); ++i)
    {
        QModelIndex index = model()->index( i, 1 );

        // menu is deleted when hidden. So do not delete the buttons.
        QToolButton* button = new QToolButton();
        button->setIcon( QIcon(":/icons/icons/eye106.png") );
        setIndexWidget( index, button );

        SetlistItem* item = model()->itemAt( index );
        const Song* song = NULL;
        if (!item || !(song = item->song()))
        {
            button->setEnabled( false );
        }
        else
        {
            QMenu* menu = new QMenu( button );
            button->setMenu( menu );
            button->setPopupMode( QToolButton::MenuButtonPopup );

            for (Attachment* attachment : song->attachments())
            {
                if ( includeAttachment( attachment ) )
                {
                    ChordPatternAttachment* cpa = qobject_assert_cast<ChordPatternAttachment*>( attachment );
                    QAction* action = new QAction(menu);
                    action->setText( cpa->name() );
                    menu->addAction( action );

                    connect( action, &QAction::triggered, [this, cpa]()
                    {
                        ChordPatternViewer::displayChordPatternAttachment( cpa, this );
                    });
                }
            }
            if (!menu->actions().isEmpty())
            {
                menu->setDefaultAction( menu->actions().first() );
            }

            connect( button, &QToolButton::clicked, [this, menu]()
            {
                if (!menu->actions().isEmpty())
                {
                    menu->actions().first()->trigger();
                }
            });

            if (song->program().valid)
            {
                button->setIcon( QIcon(":/icons/icons/eye_midi.png") );
            }
        }
    }
}

void SetlistView::setFilterTag( const QString& tag )
{
    m_filterTag = tag;
    updateCellWidgets();
}
