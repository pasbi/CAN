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

SongTableView::SongTableView(QWidget *parent) :
    QTableView(parent)
{
    verticalHeader()->hide();
    delete horizontalHeader();
    setHorizontalHeader(new RenamableHeaderView( Qt::Horizontal, this ));
    horizontalHeader()->show();


    addAction(new QAction("Hello World", this));

    setContextMenuPolicy(Qt::CustomContextMenu);

    connect(this, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(showContextMenu()));


}

void SongTableView::showContextMenu()
{
    QMenu* menu = new QMenu(this);

    setUpContextMenu(menu);

    menu->popup(QCursor::pos());
    connect(menu, SIGNAL(aboutToHide()), menu, SLOT(deleteLater()));
}

QAction* addAction( QMenu* menu, const QString & caption, std::function<void ()> do_action )
{
    QAction* action = new QAction(caption, menu);
    menu->addAction(action);
    QObject::connect(action, &QAction::triggered, do_action );
    return action;
}

QModelIndex SongTableView::indexUnderCursor() const
{
    QPoint pos = viewport()->mapFromGlobal( QCursor::pos() );
    qDebug() << model()->songAtIndex(indexAt(pos)) << indexAt(pos) << pos << QCursor::pos();
    return indexAt(pos);

}

void SongTableView::setUpContextMenu(QMenu *menu)
{
    QModelIndex index = indexUnderCursor();
    Song* song = model()->songAtIndex(index);
    // new Song
    ::addAction(menu, tr("New Song"), [this](){
       model()->project()->pushCommand( new SongDatabaseNewSongCommand( model() ) );
    });

    ::addAction(menu, tr("Delete Song"), [this, song]() {
        if (song)
        {
            model()->project()->pushCommand( new SongDatabaseRemoveSongCommand( model(), song ) );
        }
    })->setEnabled(!!song);

    menu->addSeparator();

    ::addAction(menu, "Add Attribute", [this]() {

        SongDatabaseNewAttributeCommand* naCommand = new SongDatabaseNewAttributeCommand( model() );
        model()->project()->beginMacro( naCommand->text() );
        model()->project()->pushCommand( naCommand );
        qobject_cast<RenamableHeaderView*>(horizontalHeader())->editHeader( model()->columnCount() - 1, true );
    });
}



