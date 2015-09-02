#include "eventdatabasewidget.h"
#include "ui_eventdatabasewidget.h"
#include "application.h"
#include "Project/project.h"
#include "eventtableview.h"
#include "Database/EventDatabase/eventdatabase.h"
#include "setlistwidget.h"
#include "setlistview.h"

EventDatabaseWidget::EventDatabaseWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EventDatabaseWidget)
{


    setupUi();

    ui->setupUi(this);
    m_tableViewContainer->setModel( app().project()->eventDatabaseProxy() );
    connect( m_tableViewContainer->eventTableView()->selectionModel(),
             SIGNAL(selectionChanged(QItemSelection,QItemSelection)),
             this,
             SLOT(updateSetlistView()) );
    connect( m_tableViewContainer->eventTableView()->model(), SIGNAL(modelReset()), this, SLOT(updateSetlistView()) );
}

EventDatabaseWidget::~EventDatabaseWidget()
{
    delete ui;
}

void EventDatabaseWidget::setupUi()
{
    QSplitter* splitter = new QSplitter(Qt::Horizontal, this);
    m_tableViewContainer = new EventTableViewContainer(this);   //TODO TableViewContainer<Event>
    m_setlistWidget = new SetlistWidget(splitter);
    splitter->addWidget(m_tableViewContainer);
    splitter->addWidget(m_setlistWidget);
    QHBoxLayout* layout = new QHBoxLayout(this);
    layout->addWidget(splitter);
    setLayout(layout);
}

Event* EventDatabaseWidget::currentEvent() const
{
    QModelIndexList rows = m_tableViewContainer->eventTableView()->selectionModel()->selectedRows();
    if (rows.isEmpty())
    {
        return NULL;
    }
    else
    {
        return m_tableViewContainer->eventTableView()->model()->resolveItemAtIndex( rows.first() );
    }
}

void EventDatabaseWidget::updateSetlistView()
{
    Event* e = currentEvent();
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

DatabaseView<Event>* EventDatabaseWidget::databaseView() const
{
    return m_tableViewContainer->eventTableView();
}

Setlist* EventDatabaseWidget::currentSetlist() const
{
    return m_setlistWidget->listView()->model();
}

