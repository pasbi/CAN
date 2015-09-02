#include "setlistview.h"

#include <QMimeData>
#include <QToolButton>
#include <QMenu>
#include <QAction>
#include <QHeaderView>
#include <QClipboard>

#include "application.h"
#include "Project/project.h"
#include "Commands/DatabaseCommands/databasenewitemcommand.h"
#include "Commands/DatabaseCommands/databaseremoveitemcommand.h"
#include "util.h"
#include "Dialogs/chordpatternviewer.h"
#include "Database/databasemimedata.h"
#include "Database/EventDatabase/setlistitem.h"
#include "Database/SongDatabase/song.h"
#include "Database/EventDatabase/setlist.h"
#include "setlistitemselector.h"
#include "Attachments/ChordPatternAttachment/chordpatternattachment.h"
#include "DatabaseView/ItemDelegates/lineeditdelegate.h"


SetlistView::SetlistView(QWidget *parent) :
    DatabaseView<SetlistItem>(parent)
{
    setAcceptDrops(true);
    setDropIndicatorShown( false );
    setDragDropMode( DragDrop );
    setSelectionMode( QAbstractItemView::ExtendedSelection );
    setDefaultDropAction( Qt::MoveAction );
    setDropIndicatorShown(true);
    setAlternatingRowColors( true );
    horizontalHeader()->hide();
    setHorizontalScrollMode( QTableView::ScrollPerPixel );

    setEditTriggers( QAbstractItemView::DoubleClicked | QAbstractItemView::EditKeyPressed );
    setItemDelegateForColumn( 0, new LineEditDelegate<SetlistItem>(this) );

    setContextMenuPolicy( Qt::ActionsContextMenu );
}

SetlistView::~SetlistView()
{
}

void SetlistView::mousePressEvent(QMouseEvent *event)
{
    DatabaseView::mousePressEvent( event );
    emit mousePress();
}

void SetlistView::setModel(Setlist *setlist)
{
    if (model())
    {
        disconnect( model(), SIGNAL(rowsInserted(QModelIndex,int,int)),                this, SLOT(updateCellWidgets()) );
        disconnect( model(), SIGNAL(rowsMoved(QModelIndex,int,int,QModelIndex,int)),   this, SLOT(updateCellWidgets()) );
        disconnect( model(), SIGNAL(rowsRemoved(QModelIndex,int,int)),                 this, SLOT(updateCellWidgets()) );
        disconnect( model(), SIGNAL(modelReset()),                                     this, SLOT(updateCellWidgets()) );
        disconnect( model(), SIGNAL(selectionRequest(QModelIndexList)),                this, SLOT(select(QModelIndexList)) );
    }
    QTableView::setModel( setlist );
    if (setlist)
    {
        connect( setlist, SIGNAL(rowsInserted(QModelIndex,int,int)),                this, SLOT(updateCellWidgets()) );
        connect( setlist, SIGNAL(rowsMoved(QModelIndex,int,int,QModelIndex,int)),   this, SLOT(updateCellWidgets()) );
        connect( setlist, SIGNAL(rowsRemoved(QModelIndex,int,int)),                 this, SLOT(updateCellWidgets()) );
        connect( setlist, SIGNAL(modelReset()),                                     this, SLOT(updateCellWidgets()) );
        connect( setlist, SIGNAL(selectionRequest(QModelIndexList)),                this, SLOT(select(QModelIndexList)) );
        updateCellWidgets();
        horizontalHeader()->setSectionResizeMode( 0, QHeaderView::Stretch );
        horizontalHeader()->setSectionResizeMode( 1, QHeaderView::Fixed );
        horizontalHeader()->resizeSection( 1, 60 );
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

    //TODO refactor this stuff. Think about outsoursing this with EventDatabaseView and SongDatabaseView in a common super class
    menu->addActions( actions() );
    actions()[0]->setEnabled( !!model() );                           // new item
    actions()[1]->setEnabled( !!model() && !selectionIsEmpty );      // remove item
    actions()[2]->setEnabled( !!model() && !selectionIsEmpty );      // copy items
    actions()[3]->setEnabled( !!model() && app().clipboard()->mimeData()->formats().contains( DatabaseMimeData<SetlistItem>::mimeType() ) );     // paste items
}

QList<SetlistItem*> SetlistView::selectedItems() const
{
    QList<SetlistItem*> items;
    if (model() && selectionModel())
    {
        for (const QModelIndex& index : selectionModel()->selectedRows())
        {
            items << model()->itemAtIndex( index );
        }
        return items;
    }
    else
    {
        return QList<SetlistItem*>();
    }
}

bool SetlistView::attachmentIsIgnored( const Attachment* attachment ) const
{
    // if attachment is of wrong type, do not include it.
    if ( !attachment->inherits(AbstractChordPatternAttachment::staticMetaObject.className()))
    {
        return true;
    }

    // if attachment has no tags, include it.
    else if ( attachment->tags().isEmpty() )
    {
        return false;
    }

    // if no filter tag is set, return true.
    else if ( m_filterTag.isEmpty() )
    {
        return false;
    }

    // if attachment has filter tag, include it.
    else if ( attachment->tags().contains( m_filterTag ) )
    {
        return false;
    }

    // otherwise, ignore attachment
    return true;
}

QWidget* SetlistView::createSongCellWidget(const Song* song)
{
    QToolButton* button = new QToolButton();
    button->setIcon( QIcon(":/icons/icons/eye106.png") );
    QMenu* menu = new QMenu( button );
    button->setMenu( menu );
    button->setPopupMode( QToolButton::MenuButtonPopup );

    for (Attachment* attachment : song->attachments())
    {
        if ( !attachmentIsIgnored( attachment ) )
        {
            AbstractChordPatternAttachment* cpa = qobject_assert_cast<AbstractChordPatternAttachment*>( attachment );
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
    else
    {
        button->setEnabled(false);
    }

    connect( button, &QToolButton::clicked, [this, menu]()
    {
        if (!menu->actions().isEmpty())
        {
            menu->actions().first()->trigger();
        }
    });

    if (song->program().isValid())
    {
        button->setIcon( QIcon(":/icons/icons/eye_midi.png") );
    }
    return button;
}

void SetlistView::updateCellWidgets()
{
    if (!model())
    {
        return;
    }

    clearSpans();
    for (int i = 0; i < model()->rowCount(); ++i)
    {
        QModelIndex index = model()->index( i, 1 );

        SetlistItem* item = model()->itemAtIndex( index );
        if (item)
        {
            switch (item->type())
            {
            case SetlistItem::LabelType:
                setSpan(i, 0, 1, model()->columnCount()); // there is no widget, so we don't need this column here.
                break;
            case SetlistItem::SongType:
                setIndexWidget( index, createSongCellWidget(item->song()) );
                break;
            }
        }
    }
}

void SetlistView::setFilterTag( const QString& tag )
{
    m_filterTag = tag;
    updateCellWidgets();
}

void SetlistView::select(QModelIndexList indexes)
{
    if (model())
    {
        clearSelection();

        for (const QModelIndex& index : indexes)
        {
            selectRow(index.row());
        }
    }
}
