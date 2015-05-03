#include "alternativefilemanager.h"
#include "ui_alternativefilemanager.h"

AlternativeFileManager::AlternativeFileManager(IndexedFileAttachment *attachment, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AlternativeFileManager),
    m_attachment( attachment )
{
    ui->setupUi(this);
    setupTable();
}

AlternativeFileManager::~AlternativeFileManager()
{
    delete ui;
}

void AlternativeFileManager::setupTable()
{
//    for (const IndexedFileAttachment::Alternative & a : m_attachment->alternatives())
//    {
//        int row = ui->tableWidget->rowCount();
//        ui->tableWidget->insertRow( row );

//        QTableWidgetItem* userItem = new QTableWidgetItem();
//        QTableWidgetItem* filenameItem = new QTableWidgetItem();

//        userItem->setText( a.username() );
//        filenameItem->setText( a.filename() );

//        ui->tableWidget->setItem( row, 0, userItem );
//        ui->tableWidget->setItem( row, 1, filenameItem );
//    }
}
