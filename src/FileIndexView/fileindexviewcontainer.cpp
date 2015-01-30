#include "fileindexviewcontainer.h"
#include "ui_fileindexviewcontainer.h"
#include <QFileDialog>
#include "global.h"

FileIndexViewContainer::FileIndexViewContainer(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FileIndexViewContainer)
{
    ui->setupUi(this);

    connect(ui->buttonAddFiles,  SIGNAL( clicked() ), this, SLOT( addFiles()  ));
    connect(ui->buttonAddFolder, SIGNAL( clicked() ), this, SLOT( addFolder() ));
    connect(ui->buttonRemove,    SIGNAL( clicked() ), this, SLOT( remove()    ));
    m_proxy.setFilterKeyColumn( 2 );
}

FileIndexViewContainer::~FileIndexViewContainer()
{
    delete ui;
}

void FileIndexViewContainer::setModel(FileIndex *model)
{
    m_proxy.setSourceModel(model);
    m_proxy.setFilterFixedString("");
    ui->tableView->setModel(&m_proxy);
}

void FileIndexViewContainer::addFiles()
{
    QStringList filesToAdd = QFileDialog::getOpenFileNames( this,
                                                            tr("Add files to index"),
                                                            ui->tableView->currentPath() );
    for ( const QString & entry : filesToAdd )
    {
        ui->tableView->model()->addEntry(entry);
    }
}

void FileIndexViewContainer::addFolder()
{
    QString pathToAdd = QFileDialog::getExistingDirectory( this,
                                                           tr("Add folder to index"),
                                                           ui->tableView->currentPath() );
    ui->tableView->model()->addRecursive(pathToAdd);
}

void FileIndexViewContainer::remove()
{
    QModelIndexList selection = ui->tableView->selectionModel()->selectedRows();

    if (!selection.isEmpty())
    {
        int start = selection.first().row();
        int end = selection.last().row();

        ui->tableView->model()->removeRows(start, end - start + 1, QModelIndex());
    }
}
