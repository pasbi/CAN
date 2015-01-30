#include "fileindexviewcontainer.h"
#include "ui_fileindexviewcontainer.h"
#include <QFileDialog>

FileIndexViewContainer::FileIndexViewContainer(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FileIndexViewContainer)
{
    ui->setupUi(this);

    connect(ui->buttonAddFiles,  SIGNAL( clicked() ), this, SLOT( addFiles()  ));
    connect(ui->buttonAddFolder, SIGNAL( clicked() ), this, SLOT( addFolder() ));
    connect(ui->buttonRemove,    SIGNAL( clicked() ), this, SLOT( remove()    ));

}

FileIndexViewContainer::~FileIndexViewContainer()
{
    delete ui;
}

void FileIndexViewContainer::setModel(FileIndex *model)
{
    ui->treeView->setModel(model);
}

void FileIndexViewContainer::addFiles()
{
    QStringList filesToAdd = QFileDialog::getOpenFileNames( this,
                                                            tr("Add files to index"),
                                                            ui->treeView->currentPath() );
    for ( const QString & entry : filesToAdd )
    {
        ui->treeView->model()->addEntry(entry);
    }
}

void FileIndexViewContainer::addFolder()
{
    QString pathToAdd = QFileDialog::getExistingDirectory( this,
                                                           tr("Add folder to index"),
                                                           ui->treeView->currentPath() );
    ui->treeView->model()->addRecursive(pathToAdd);
}

void FileIndexViewContainer::remove()
{

}

