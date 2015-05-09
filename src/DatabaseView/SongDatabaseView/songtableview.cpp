#include "songtableview.h"
#include "QHeaderView"
#include <QAction>
#include "global.h"
#include "project.h"
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

    connect(horizontalHeader(), SIGNAL(sectionMoved(int,int,int)), this, SLOT(fakeFocusOutEvent()));

    verticalHeader()->hide();

    setDragDropMode( QAbstractItemView::DragDrop );
    setDragEnabled( true );
    setDropIndicatorShown( true );

    setContextMenuPolicy( Qt::NoContextMenu );

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

void SongTableView::setReadOnly()
{
    qobject_assert_cast<RenamableHeaderView*>( horizontalHeader() )->setReadOnly();
    setEditTriggers( QAbstractItemView::NoEditTriggers );
}

Qt::DropAction SongTableView::dropAction( QDropEvent* event )
{
    switch (event->keyboardModifiers())
    {
    case Qt::ControlModifier:
        return Qt::CopyAction;
    default:
        return Qt::MoveAction;
    }
}

void SongTableView::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasFormat(SongDatabase::SONG_POINTERS_MIME_DATA_FORMAT) )
    {
        event->setDropAction( dropAction( event ) );
        event->accept();
    }
}

void SongTableView::dragMoveEvent(QDragMoveEvent *event)
{
    if (event->mimeData()->hasFormat(SongDatabase::SONG_POINTERS_MIME_DATA_FORMAT) )
    {
        event->setDropAction( dropAction( event ) );
        event->accept();
    }
}

#include "Commands/SongDatabaseCommands/songdatabaseremovesongcommand.h"
void SongTableView::dropEvent(QDropEvent *event)
{
    int row;
    QModelIndex index = indexAt( event->pos() );
    if (index.isValid())
    {
        row = proxyModel()->mapToSource( index ).row();
    }
    else
    {
        row = model()->rowCount();
    }

    pasteSongs( event->mimeData(), row, dropAction( event ) );
}

#include "Commands/SongDatabaseCommands/songdatabasemovesongcommand.h"
#include "Commands/SongDatabaseCommands/songdatabasenewsongcommand.h"
void SongTableView::pasteSongs(const QMimeData *mimeData, int row, Qt::DropAction action)
{
    QByteArray data = mimeData->data( SongDatabase::SONG_POINTERS_MIME_DATA_FORMAT );
    if (data.isEmpty())
    {
        return;
    }

    QList<qintptr> originalSongs;
    QDataStream stream( data );
    stream >> originalSongs;

    app().beginMacro( tr("Paste songs") );
    if (action == Qt::MoveAction)
    {
        for (qintptr song : originalSongs)
        {
            QModelIndex index = model()->indexOfSong((Song*) song);
            index = proxyModel()->mapFromSource( index );
            int sourceRow = proxyModel()->mapToSource( index ).row();

            app().pushCommand( new SongDatabaseMoveSongCommand( model(), sourceRow, row ) );
        }
    }
    else if (action == Qt::CopyAction)
    {
        for (qintptr song : originalSongs)
        {
            app().pushCommand( new SongDatabaseNewSongCommand( model(), ((Song*) song)->copy() ) );
        }
    }
    app().endMacro();
}



