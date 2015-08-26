#include "eventtableviewcontainer.h"
#include "ui_eventtableviewcontainer.h"

EventTableViewContainer::EventTableViewContainer(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EventTableViewContainer)
{
    ui->setupUi(this);

}

EventTableViewContainer::~EventTableViewContainer()
{
    delete ui;
}

EventTableView* EventTableViewContainer::eventTableView() const
{
    return ui->tableView;
}

void EventTableViewContainer::setModel(EventDatabaseSortProxy *model )
{
    ui->tableView->setModel( model );
    connect(ui->filterWidget, SIGNAL(filterChanged(QString)), model, SLOT(setFilterWildcard(QString)));
}

Event* EventTableViewContainer::currentEvent() const
{
    QModelIndexList rows = ui->tableView->selectionModel()->selectedRows();
    if (rows.isEmpty())
    {
        return NULL;
    }
    return ui->tableView->model()->itemAtIndex( rows.first() );
}
