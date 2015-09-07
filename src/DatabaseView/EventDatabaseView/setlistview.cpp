#include "setlistview.h"

#include <QMimeData>
#include <QToolButton>
#include <QMenu>
#include <QAction>
#include <QHeaderView>
#include <QClipboard>
#include <QKeyEvent>

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
#include "DatabaseView/ItemDelegates/setlistviewitemdelegate.h"
#include "Database/EventDatabase/setlistproxy.h"


SetlistView::SetlistView(QWidget *parent) :
    DatabaseView( new SetlistProxy(), parent)
{
    setAcceptDrops(true);
    setDropIndicatorShown( false );
    setDragDropMode( DragDrop );
    setDefaultDropAction( Qt::MoveAction );
    setDropIndicatorShown(true);
    setAlternatingRowColors( true );
    horizontalHeader()->hide();

    setEditTriggers( QAbstractItemView::DoubleClicked | QAbstractItemView::EditKeyPressed );

    setContextMenuPolicy( Qt::ActionsContextMenu );
    setItemDelegateForColumn(0, new SetlistViewItemDelegate(this));

    setSelectionMode( QAbstractItemView::ExtendedSelection );

    connect(this, SIGNAL(changed()), this, SLOT(updateCellWidgets()));
}

void SetlistView::setModel(Database<SetlistItem> *setlist)
{
    if (sourceModel())
    {
        disconnect( sourceModel(), SIGNAL(selectionRequest(QModelIndexList)),          this, SLOT(select(QModelIndexList)) );
    }
    DatabaseView<SetlistItem>::setModel( setlist );
    if (setlist)
    {
        connect( setlist, SIGNAL(selectionRequest(QModelIndexList)),                this, SLOT(select(QModelIndexList)) );
        updateCellWidgets();

        horizontalHeader()->setSectionResizeMode( 0, QHeaderView::Stretch );
        horizontalHeader()->setSectionResizeMode( 1, QHeaderView::Fixed );
        horizontalHeader()->resizeSection( 1, 60 );
    }
}

QList<SetlistItem*> SetlistView::selectedItems() const
{
    QList<SetlistItem*> items;
    if (model() && selectionModel())
    {
        for (const QModelIndex& index : selectionModel()->selectedRows())
        {
            items << itemAtIndex( index );
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
    QToolButton* button = new QToolButton();    // will be owned by calling method
    button->setIcon( QIcon(":/icons/icons/eye106.png") );
    QMenu* menu = new QMenu( button );
    button->setMenu( menu );
    button->setPopupMode( QToolButton::InstantPopup );

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

        SetlistItem* item = itemAtIndex( index );
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
        QList<int> rows;
        for (const QModelIndex& index : indexes)
        {
            if (!rows.contains(index.row()))
            {
                rows << index.row();
            }
        }

        clearSelection();

        for (const QModelIndex& index : indexes)
        {
            setSelectionMode( QAbstractItemView::MultiSelection );
            selectRow(index.row());
            setSelectionMode( QAbstractItemView::ExtendedSelection );
        }
    }
}
