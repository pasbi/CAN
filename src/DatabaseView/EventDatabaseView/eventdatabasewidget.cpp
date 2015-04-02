#include "eventdatabasewidget.h"
#include "ui_eventdatabasewidget.h"

EventDatabaseWidget::EventDatabaseWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EventDatabaseWidget)
{
    ui->setupUi(this);
    connect( ui->eventTableViewContainer->eventTableView(), SIGNAL(pressed(QModelIndex)), this, SLOT(updateSetlistView()) );

}

EventDatabaseWidget::~EventDatabaseWidget()
{
    delete ui;
}

void EventDatabaseWidget::setEventDatabase( EventDatabase * eventDatabase )
{
    m_eventDatabase = eventDatabase;
    m_sortFilterProxy.setSourceModel( eventDatabase );
    ui->eventTableViewContainer->setModel( &m_sortFilterProxy );
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
