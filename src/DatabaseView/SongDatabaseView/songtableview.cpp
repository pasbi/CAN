#include "songtableview.h"
#include "QHeaderView"
#include "global.h"
#include "Project/project.h"
#include "util.h"
#include <QFocusEvent>
#include "application.h"
#include "songattributedelegate.h"
#include "Database/SongDatabase/songdatabasesortproxy.h"


SongTableView::SongTableView(QWidget *parent) :
    DatabaseView(parent),
    m_delegate( new SongAttributeDelegate(this) )
{
    verticalHeader()->hide();

    horizontalHeader()->show();
    horizontalHeader()->setSortIndicatorShown( true );
    horizontalHeader()->setSectionsClickable( true );

    setItemDelegate( m_delegate );

    setSelectionMode(QAbstractItemView::SingleSelection);
    setSelectionBehavior(QAbstractItemView::SelectRows);

    connect(horizontalHeader(), SIGNAL(sectionMoved(int,int,int)), this, SLOT(fakeFocusOutEvent()));
    setSortingEnabled( true );

    setDragEnabled(true);
}

void SongTableView::setModel(SongDatabaseSortProxy *model)
{
    DatabaseView::setModel(model);
    connect( model, SIGNAL(modelReset()), this, SLOT(resizeColumnsToContents()) );
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



SongDatabaseSortProxy* SongTableView::proxyModel() const
{
    SongDatabaseSortProxy* pm = qobject_assert_cast<SongDatabaseSortProxy*>( QTableView::model() );
    assert( pm == QTableView::model() );
    return pm;
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

void SongTableView::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Tab)
    {
        if ( m_delegate->isEditing() )
        {
            QTableView::keyPressEvent( event ); // sets the current index to the next item
            edit( currentIndex() );             // start editing (closes editor automatically when finished)
            return;                             // return to prevent interpreting the tab event further.
        }
    }
    QTableView::keyPressEvent( event );
}

Database<Song>* SongTableView::model() const
{
    return proxyModel() ? proxyModel()->sourceModel() : NULL;
}
