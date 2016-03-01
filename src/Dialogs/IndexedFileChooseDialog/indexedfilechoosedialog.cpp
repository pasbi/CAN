#include "indexedfilechoosedialog.h"
#include "ui_indexedfilechoosedialog.h"
#include "application.h"
#include <qmath.h>
#include <QFileDialog>
#include <QMessageBox>
#include "util.h"
#include "Attachments/AudioAttachment/sectionsmodel.h"
#include "Database/SongDatabase/song.h"
#include "filefilter.h"
#include "FileIndex/fileindex.h"


IndexedFileChooseDialog::IndexedFileChooseDialog(const Song *song, const QByteArray &currentHash, const QStringList &endings, QWidget *parent) :
    QDialog(parent),
    ui( new Ui::IndexedFileChooseDialog ),
    m_song( song ),
    m_hash( currentHash ),
    m_acceptedEndings( endings ),
    m_fileFilter( new FileFilter() )
{
    ui->setupUi(this);
    m_filenames = gatherFiles();
    int i = 0;
    ui->tableWidget->insertColumn(0);
    ui->tableWidget->insertColumn(1);
    for (const QString & filename : m_filenames )
    {
        ui->tableWidget->insertRow( i );

        QTableWidgetItem* filepathItem = new QTableWidgetItem( filename );
        filepathItem->setFlags( Qt::ItemIsSelectable | Qt::ItemIsEnabled );
        QTableWidgetItem* filenameItem = new QTableWidgetItem( QFileInfo(filename).completeBaseName() );
        filenameItem->setFlags( Qt::ItemIsSelectable | Qt::ItemIsEnabled );

        ui->tableWidget->setItem( i, 0, filenameItem );
        ui->tableWidget->setItem( i, 1, filepathItem );
        i++;
    }
    ui->tableWidget->horizontalHeader()->resizeSections( QHeaderView::ResizeToContents );
    connect( ui->tableWidget->selectionModel(),
             SIGNAL(selectionChanged(QItemSelection,QItemSelection)),
             this,
             SLOT(onSelectionChanged(QItemSelection, QItemSelection))  );

    setFilename( currentFilename() );

    connect( ui->pushButtonClearEdit, SIGNAL(clicked()), this, SLOT(clearFilename()) );
}

IndexedFileChooseDialog::~IndexedFileChooseDialog()
{
    delete ui;
    delete m_fileFilter;
    m_fileFilter = nullptr;
}

QStringList IndexedFileChooseDialog::gatherFiles()
{
    QStringList filenames = app().fileIndex()->filenames();
    return m_fileFilter->apply(filenames,
                               QStringList( {
                                   m_song->attribute("title").toString(),
                                   m_song->attribute("artist").toString()
                               } ),
                               m_acceptedEndings );
}

void IndexedFileChooseDialog::setFilename(const QString &filename)
{
    ui->tableWidget->blockSignals(true);
    if (filename.isEmpty())
    {
        m_hash.clear();
        ui->tableWidget->selectionModel()->clear();
    }
    else
    {
        QByteArray hash = app().fileIndex()->hash( filename );

        m_hash = hash;

        ui->lineEdit->setText( filename );

        int row = m_filenames.indexOf( filename );
        if (row < 0)
        {
            ui->tableWidget->selectionModel()->clear();
        }
        else
        {
            ui->tableWidget->selectRow( row );
        }

    }
    ui->tableWidget->blockSignals(false);
}

QString IndexedFileChooseDialog::currentFilename()
{
    return app().fileIndex()->filename( m_hash );
}

void IndexedFileChooseDialog::on_pushButtonOpenFileDialog_clicked()
{
    QString filename = currentFilename();
    if (filename.isEmpty())
    {
        filename = QDir::homePath();
    }

    QString filter = "(";
    for (const QString& ending : m_acceptedEndings)
    {
        filter.append(QString("*.%1 ").arg(ending));
    }
    if (filter.endsWith(" "))
    {
        filter = filter.left( filter.length() - 1 );
    }
    filter.append(")");

    filename = QFileDialog::getOpenFileName( this, tr("Open File ..."), filename, filter );
    if (filename.isEmpty())
    {
        return;
    }
    else
    {
        setFilename( filename );
    }
}

void IndexedFileChooseDialog::on_lineEdit_editingFinished()
{
    QString filename = ui->lineEdit->text();
    if (QFileInfo(filename).isReadable() || filename.isEmpty())
    {
        setFilename( filename );
    }
    else
    {
        QMessageBox::warning( this,
                              tr("File is not readable."),
                              tr("The file is not readable.\n"),
                              QMessageBox::Ok,
                              QMessageBox::NoButton );
        m_hash.clear();
    }
}

void IndexedFileChooseDialog::reject()
{
    m_hash.clear();
    QDialog::reject();
}

void IndexedFileChooseDialog::accept()
{
    if (!ui->lineEdit->text().isEmpty() && m_hash.isEmpty())
    {
        return;
    }
    else
    {
        QDialog::accept();
    }
}

void IndexedFileChooseDialog::onSelectionChanged(QItemSelection selected, QItemSelection deselected)
{
    Q_UNUSED( deselected );
    int row = -1;
    QModelIndexList rows = selected.indexes();
    if (!rows.isEmpty())
    {
        row = rows.first().row();
    }

    if (row >= 0)
    {
        setFilename( m_filenames[row] );
    }
    else
    {
        setFilename( "" );
    }
}

void IndexedFileChooseDialog::clearFilename()
{
    setFilename( "" );
    ui->lineEdit->clear();
    ui->tableWidget->selectionModel()->clearSelection();
}
