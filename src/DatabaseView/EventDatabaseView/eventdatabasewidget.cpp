#include "eventdatabasewidget.h"
#include "application.h"
#include "Project/project.h"
#include "eventtableview.h"
#include "Database/EventDatabase/eventdatabase.h"
#include "setlistwidget.h"
#include "setlistview.h"
#include "Database/EventDatabase/setlist.h"
#include "Database/EventDatabase/eventdatabasesortproxy.h"

EventDatabaseWidget::EventDatabaseWidget(QWidget *parent) :
    DatabaseWidget(new EventTableView(), new SetlistWidget(), parent)
{
    connect( databaseView()->selectionModel(),
             SIGNAL(selectionChanged(QItemSelection,QItemSelection)),
             this,
             SLOT(updateSetlistView()) );
    connect( databaseView()->model(), SIGNAL(modelReset()), this, SLOT(updateSetlistView()) );
}

SetlistWidget* EventDatabaseWidget::setlistWidget() const
{
    return m_secondWidget;
}

void EventDatabaseWidget::updateSetlistView()
{
    Event* e = currentItem();
    if (e)
    {
        setlistWidget()->setSetlist( e->setlist() );
    }
    else
    {
        setlistWidget()->setSetlist( nullptr );
    }
}

SetlistView* EventDatabaseWidget::setlistView() const
{
    return setlistWidget()->setlistView();
}
