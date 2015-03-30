#include "eventdatabasewidget.h"
#include "ui_eventdatabasewidget.h"

EventDatabaseWidget::EventDatabaseWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EventDatabaseWidget)
{
    ui->setupUi(this);
}

EventDatabaseWidget::~EventDatabaseWidget()
{
    delete ui;
    // delete m_sortFilterProxy;
}

void EventDatabaseWidget::setEventDatabase( EventDatabase * eventDatabase )
{
    m_dateDatabase = eventDatabase;
    m_sortFilterProxy.setSourceModel( eventDatabase );
    ui->tableView->setModel( &m_sortFilterProxy );

//    connect( ui->tableView->selectionModel(),
//             SIGNAL(currentRowChanged(QModelIndex,QModelIndex)),
//             this,
//             SLOT( updateAttachmentChooser(QModelIndex, QModelIndex) ));

}

Event* EventDatabaseWidget::currentEvent() const
{
    QModelIndexList rows = ui->tableView->selectionModel()->selectedRows();
    if (rows.isEmpty())
    {
        return NULL;
    }
    else
    {
        return ui->tableView->model()->eventAtIndex( rows.first() );
    }
}
