#include "eventdatabasewidget.h"
#include "ui_eventdatabasewidget.h"
#include "application.h"
#include "project.h"

EventDatabaseWidget::EventDatabaseWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EventDatabaseWidget)
{
    ui->setupUi(this);
    connect( ui->eventTableViewContainer->eventTableView(), SIGNAL(pressed(QModelIndex)), this, SLOT(updateSetlistView()) );

    ui->eventTableViewContainer->setModel( app().project()->eventDatabaseProxy() );
}

EventDatabaseWidget::~EventDatabaseWidget()
{
    delete ui;
}

Event* EventDatabaseWidget::currentEvent() const
{
    QModelIndexList rows = ui->eventTableViewContainer->eventTableView()->selectionModel()->selectedRows();
    if (rows.isEmpty())
    {
        return NULL;
    }
    else
    {
        return ui->eventTableViewContainer->eventTableView()->model()->eventAtIndex( rows.first() );
    }
}

void EventDatabaseWidget::updateSetlistView()
{
    Event* e = currentEvent();
    if (e)
    {
        ui->setlistWidget->setSetlist( e->setlist() );
    }
    else
    {
        ui->setlistWidget->setSetlist( NULL );
    }
}

QListView* EventDatabaseWidget::setlistView() const
{
    return ui->setlistWidget->listView();
}

EventTableView* EventDatabaseWidget::eventTableView() const
{
    return ui->eventTableViewContainer->eventTableView();
}

SetlistItem* EventDatabaseWidget::currentSetlistItem() const
{
    return ui->setlistWidget->currentItem();
}

Setlist* EventDatabaseWidget::currentSetlist() const
{
    return qobject_assert_cast<Setlist*>( ui->setlistWidget->listView()->model() );
}

