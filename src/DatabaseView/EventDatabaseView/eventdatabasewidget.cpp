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
    DatabaseWidget(parent)
{
    setupUi();

    m_databaseViewContainer->setDatabase( app().project()->eventDatabaseProxy() );
    connect( m_databaseViewContainer->databaseView()->selectionModel(),
             SIGNAL(selectionChanged(QItemSelection,QItemSelection)),
             this,
             SLOT(updateSetlistView()) );
    connect( m_databaseViewContainer->databaseView()->model(), SIGNAL(modelReset()), this, SLOT(updateSetlistView()) );
}

void EventDatabaseWidget::setupUi()
{
    QSplitter* splitter = new QSplitter(Qt::Horizontal, this);
    m_databaseViewContainer = new EventTableViewContainer(this);   //TODO TableViewContainer<Event>
    m_setlistWidget = new SetlistWidget(splitter);
    splitter->addWidget(m_databaseViewContainer);
    splitter->addWidget(m_setlistWidget);
    QHBoxLayout* layout = new QHBoxLayout(this);
    layout->addWidget(splitter);
    setLayout(layout);
}

void EventDatabaseWidget::updateSetlistView()
{
    Event* e = currentItem();
    if (e)
    {
        m_setlistWidget->setSetlist( e->setlist() );
    }
    else
    {
        m_setlistWidget->setSetlist( NULL );
    }
}

SetlistView* EventDatabaseWidget::setlistView() const
{
    return m_setlistWidget->listView();
}

Setlist* EventDatabaseWidget::currentSetlist() const
{
    return m_setlistWidget->listView()->model();
}

