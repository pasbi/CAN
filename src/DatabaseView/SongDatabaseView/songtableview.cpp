#include "songtableview.h"
#include "QHeaderView"
#include <QAction>
#include "global.h"
#include "project.h"
#include "Commands/SongDatabaseCommands/songdatabasenewsongcommand.h"
#include "Commands/SongDatabaseCommands/songdatabaseremovesongcommand.h"
#include "Commands/SongDatabaseCommands/songdatabasenewattributecommand.h"
#include "renamableheaderview.h"
#include "util.h"
#include <QFocusEvent>
#include "application.h"


SongTableView::SongTableView(QWidget *parent) :
    DatabaseView(parent),
    m_delegate( new SongAttributeDelegate(this) )
{
    delete horizontalHeader();
    setHorizontalHeader(new RenamableHeaderView( Qt::Horizontal, this ));
    horizontalHeader()->show();

    setItemDelegate( m_delegate );

    setSelectionMode(QAbstractItemView::SingleSelection);
    setSelectionBehavior(QAbstractItemView::SelectRows);

    setAlternatingRowColors(true);

    horizontalHeader()->setSectionsMovable(true);
    horizontalHeader()->setDragEnabled(true);
    horizontalHeader()->setDragDropMode(QAbstractItemView::InternalMove);
    horizontalHeader()->setSortIndicatorShown( true );
    horizontalHeader()->setSectionsClickable( true );
    connect(horizontalHeader(), SIGNAL(sectionMoved(int,int,int)), this, SLOT(fakeFocusOutEvent()));

    verticalHeader()->setSectionsMovable(true);
    verticalHeader()->setDragEnabled(true);
    verticalHeader()->setDragDropMode(QAbstractItemView::InternalMove);
    connect(verticalHeader(), SIGNAL(sectionMoved(int,int,int)), this, SLOT(fakeFocusOutEvent()));

    setSortingEnabled( true );

    setDragDropMode( QAbstractItemView::DragOnly );
    setDragEnabled( true );
    setDropIndicatorShown( true );

}

void SongTableView::setUpContextMenu(QMenu *menu)
{
    QModelIndex index = indexUnderCursor();
    Song* song = model()->songAtIndex(index);
    // new Song
    Util::addAction(menu, tr("New Song"), [this](){
        app().pushCommand( new SongDatabaseNewSongCommand( model(), new Song(model()) ) );
    });

    Util::addAction(menu, tr("Delete Song"), [this, song]() {
        if (song)
        {
            app().pushCommand( new SongDatabaseRemoveSongCommand( model(), song ) );
        }
    })->setEnabled(!!song);

    menu->addSeparator();

    Util::addAction(menu, tr("Add Attribute"), [this]() {
        SongDatabaseNewAttributeCommand* naCommand = new SongDatabaseNewAttributeCommand( model() );

        // end macro is in renamableheaderview.cpp
        model()->project()->beginMacro( naCommand->text() );
        model()->project()->pushCommand( naCommand );
        qobject_assert_cast<RenamableHeaderView*>(horizontalHeader())->editHeader( model()->columnCount() - 1, true );
    });
}

void SongTableView::setModel(SongDatabaseSortProxy *model)
{
    QTableView::setModel(model);
}

void SongTableView::fakeFocusOutEvent()
{
    // oddly, the columns in the table are not resized when the header is resized.
    // though, the colmns are resized when table loses focus.
    // since table can lose focus safely on resize, fire appropriate event to reach the
    // desired behaviour on the wrong track.
    QFocusEvent myFocusOutEvent(QEvent::FocusOut);
    QApplication::sendEvent( this, &myFocusOutEvent );
}

SongDatabase* SongTableView::model() const
{
    return proxyModel() ? proxyModel()->sourceModel() : NULL;
}

SongDatabaseSortProxy* SongTableView::proxyModel() const
{
    SongDatabaseSortProxy* pm = qobject_assert_cast<SongDatabaseSortProxy*>( QTableView::model() );
    assert( pm == QTableView::model() );
    return pm;
}



