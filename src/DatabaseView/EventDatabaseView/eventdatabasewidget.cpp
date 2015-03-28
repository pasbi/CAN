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

void EventDatabaseWidget::setEventDatabase( EventDatabase * dateDatabase )
{
    m_dateDatabase = dateDatabase;
//    m_sortFilterProxy.setSourceModel( dateDatabase );
    ui->tableView->setModel( dateDatabase );

//    connect( ui->tableView->selectionModel(),
//             SIGNAL(currentRowChanged(QModelIndex,QModelIndex)),
//             this,
//             SLOT( updateAttachmentChooser(QModelIndex, QModelIndex) ));

}
