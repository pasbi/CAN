#include "fileindexviewcontainer.h"
#include "ui_fileindexviewcontainer.h"
#include <QFileDialog>
#include "global.h"
#include <QLineEdit>
#include "progressdialog.h"

FileIndexViewContainer::FileIndexViewContainer(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FileIndexViewContainer)
{
    ui->setupUi(this);

    connect(ui->buttonAddFiles,  SIGNAL( clicked() ), this, SLOT( addFiles()  ));
    connect(ui->buttonAddFolder, SIGNAL( clicked() ), this, SLOT( addFolder() ));
    connect(ui->buttonRemove,    SIGNAL( clicked() ), this, SLOT( remove()    ));
    m_proxy.setFilterKeyColumn( 2 );
    ui->comboBox->lineEdit()->setPlaceholderText(tr("Filter"));
}

FileIndexViewContainer::~FileIndexViewContainer()
{
    delete ui;
}

void FileIndexViewContainer::setModel(FileIndex *model)
{
    m_proxy.setSourceModel(model);
    model->restore();
    m_proxy.setFilterFixedString("");
    ui->tableView->setModel(&m_proxy);
}

QString FileIndexViewContainer::defautDirectory() const
{
    QString current = ui->tableView->currentPath();
    if (current.isEmpty())
    {
        return QDir::homePath();
    }
    else
    {
        return current;
    }
}

void FileIndexViewContainer::addFiles()
{
    QStringList filesToAdd = QFileDialog::getOpenFileNames( this,
                                                            tr("Add files to index"),
                                                            defautDirectory()         );
    for ( const QString & entry : filesToAdd )
    {
        ui->tableView->model()->addEntry(entry);
    }
    ui->tableView->model()->save();
}

void FileIndexViewContainer::addFolder()
{
    QString pathToAdd = QFileDialog::getExistingDirectory( this,
                                                           tr("Add folder to index"),
                                                           defautDirectory()           );

    ui->tableView->model()->setFilter( ui->comboBox->currentText() );

    ProgressDialog::show();
    if (!pathToAdd.isEmpty())
    {
        ui->tableView->model()->addRecursive(pathToAdd);
    }
    ProgressDialog::hide();
    ui->tableView->model()->save();
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
    ui->tableView->model()->save();
}
