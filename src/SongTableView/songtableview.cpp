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


SongTableView::SongTableView(QWidget *parent) :
    QTableView(parent),
    m_delegate( new SongAttributeDelegate(this) )
{
    verticalHeader()->hide();
    delete horizontalHeader();
    setHorizontalHeader(new RenamableHeaderView( Qt::Horizontal, this ));
    horizontalHeader()->show();


    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(showContextMenu(QPoint)));

    setItemDelegate( m_delegate );

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
       model()->project()->pushCommand( new SongDatabaseNewSongCommand( model() ) );
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

void SongTableView::focusOutEvent(QFocusEvent *event)
{
    qDebug() << "focusus out";

   QTableView::focusOutEvent(event);
}



