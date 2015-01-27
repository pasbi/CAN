#include "songtableview.h"
#include "QHeaderView"
#include <QMenu>
#include <QAction>
#include "global.h"
#include "project.h"
#include "Commands/SongDatabaseCommands/songdatabasenewsongcommand.h"
#include "Commands/SongDatabaseCommands/songdatabaseremovesongcommand.h"
#include "Commands/SongDatabaseCommands/songdatabasenewattributecommand.h"
#include "renamableheaderview.h"
#include <functional>
#include "util.h"
#include <QFocusEvent>
#include <QApplication>


SongTableView::SongTableView(QWidget *parent) :
    QTableView(parent),
    m_delegate( new SongAttributeDelegate(this) )
{
    delete horizontalHeader();
    setHorizontalHeader(new RenamableHeaderView( Qt::Horizontal, this ));
    horizontalHeader()->show();


    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(showContextMenu(QPoint)));

    //setItemDelegate( m_delegate );

    setSelectionMode(QAbstractItemView::SingleSelection);
    setSelectionBehavior(QAbstractItemView::SelectRows);

    setAlternatingRowColors(true);

    // todo fire focusout event on section move
    horizontalHeader()->setSectionsMovable(true);
    horizontalHeader()->setDragEnabled(true);
    horizontalHeader()->setDragDropMode(QAbstractItemView::InternalMove);
    connect(horizontalHeader(), SIGNAL(sectionMoved(int,int,int)), this, SLOT(fakeFocusOutEvent()));

    verticalHeader()->setSectionsMovable(true);
    verticalHeader()->setDragEnabled(true);
    verticalHeader()->setDragDropMode(QAbstractItemView::InternalMove);
    connect(verticalHeader(), SIGNAL(sectionMoved(int,int,int)), this, SLOT(fakeFocusOutEvent()));

}

void SongTableView::showContextMenu(QPoint pos)
{
    QMenu* menu = new QMenu(this);

    setUpContextMenu(menu);

    menu->popup(viewport()->mapToGlobal(pos));
    connect(menu, SIGNAL(aboutToHide()), menu, SLOT(deleteLater()));
}

QModelIndex SongTableView::indexUnderCursor() const
{
    QPoint pos = viewport()->mapFromGlobal( QCursor::pos() );
    return indexAt(pos);
}

void SongTableView::setUpContextMenu(QMenu *menu)
{
    QModelIndex index = indexUnderCursor();
    Song* song = model()->songAtIndex(index);
    // new Song
    Util::addAction(menu, tr("New Song"), [this](){
        model()->project()->pushCommand( new SongDatabaseNewSongCommand( model(), new Song(model()) ) );
    });

    Util::addAction(menu, tr("Delete Song"), [this, song]() {
        if (song)
        {
            model()->project()->pushCommand( new SongDatabaseRemoveSongCommand( model(), song ) );
        }
    })->setEnabled(!!song);

    menu->addSeparator();

    Util::addAction(menu, "Add Attribute", [this]() {

        SongDatabaseNewAttributeCommand* naCommand = new SongDatabaseNewAttributeCommand( model() );
        model()->project()->beginMacro( naCommand->text() );
        model()->project()->pushCommand( naCommand );
        qobject_cast<RenamableHeaderView*>(horizontalHeader())->editHeader( model()->columnCount() - 1, true );
    });
}

void SongTableView::setModel(SongDatabaseSortProxy* model)
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


