#include "eventdatabasewidget.h"
#include "ui_eventdatabasewidget.h"
#include "application.h"
#include "Project/project.h"
#include "eventtableview.h"
#include "Database/EventDatabase/eventdatabase.h"
#include "setlistwidget.h"
#include "setlistview.h"
#include "Database/EventDatabase/eventdatabasesortproxy.h"

EventDatabaseWidget::EventDatabaseWidget(QWidget *parent) :
    DatabaseWidget(new EventTableViewContainer(), new SetlistWidget(), parent)
{
    m_databaseViewContainer->setDatabase( app().project()->eventDatabaseProxy() );
    connect( m_databaseViewContainer->databaseView()->selectionModel(),
             SIGNAL(selectionChanged(QItemSelection,QItemSelection)),
             this,
             SLOT(updateSetlistView()) );
    connect( m_databaseViewContainer->databaseView()->model(), SIGNAL(modelReset()), this, SLOT(updateSetlistView()) );
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
        setlistWidget()->setSetlist( NULL );
    }
}

SetlistView* EventDatabaseWidget::setlistView() const
{
    return setlistWidget()->listView();
}

Setlist* EventDatabaseWidget::currentSetlist() const
{
    return setlistWidget()->listView()->model();
}

