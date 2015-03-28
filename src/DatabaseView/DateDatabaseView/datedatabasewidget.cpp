#include "datedatabasewidget.h"
#include "ui_datedatabasewidget.h"

DateDatabaseWidget::DateDatabaseWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DateDatabaseWidget)
{
    ui->setupUi(this);
}

DateDatabaseWidget::~DateDatabaseWidget()
{
    delete ui;
    // delete m_sortFilterProxy;
}

void DateDatabaseWidget::setDateDatabase( DateDatabase * dateDatabase )
{
    m_dateDatabase = dateDatabase;
//    m_sortFilterProxy.setSourceModel( dateDatabase );
    ui->tableView->setModel( dateDatabase );

//    connect( ui->tableView->selectionModel(),
//             SIGNAL(currentRowChanged(QModelIndex,QModelIndex)),
//             this,
//             SLOT( updateAttachmentChooser(QModelIndex, QModelIndex) ));

}
