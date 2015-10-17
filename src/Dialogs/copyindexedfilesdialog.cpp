#include "copyindexedfilesdialog.h"
#include "ui_copyindexedfilesdialog.h"
#include "Database/SongDatabase/song.h"
#include "Attachments/indexedfileattachment.h"
#include <QFileInfo>

CopyIndexedFilesDialog::CopyIndexedFilesDialog(QList<Song *> songs, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CopyIndexedFilesDialog)
{
    ui->setupUi(this);
    setupTable(songs);

    ui->tableWidget->setContextMenuPolicy(Qt::ActionsContextMenu);

    QAction* selectAllAction = new QAction(ui->tableWidget);
    selectAllAction->setText(tr("Select All"));
    connect(selectAllAction, &QAction::triggered, [this]()
    {
        select(All);
    });

    QAction* deselectAllAction = new QAction(ui->tableWidget);
    deselectAllAction->setText(tr("Deselect All"));
    connect(deselectAllAction, &QAction::triggered, [this]()
    {
        select(None);
    });

    QAction* toggleSelectionAction = new QAction(ui->tableWidget);
    toggleSelectionAction->setText(tr("Toggle Selection"));
    connect(toggleSelectionAction, &QAction::triggered, [this]()
    {
        select(Toggle);
    });

    ui->tableWidget->addActions({selectAllAction, deselectAllAction, toggleSelectionAction});
}

CopyIndexedFilesDialog::~CopyIndexedFilesDialog()
{
    delete ui;
}

void CopyIndexedFilesDialog::setupTable(QList<Song*> songs)
{
    QStringList filenames;
    for (const Song* song : songs)
    {
        for (const Attachment* a : song->attachments())
        {
            const IndexedFileAttachment* ifa = qobject_cast<const IndexedFileAttachment*>(a);
            if (ifa)
            {
                QString filename = ifa->filename();
                if (!filename.isEmpty())
                {
                    filenames << filename;
                }
            }
        }
    }

    ui->tableWidget->clear();
    ui->tableWidget->insertColumn(0);
    ui->tableWidget->insertColumn(1);
    ui->tableWidget->insertColumn(2);
    ui->tableWidget->setHorizontalHeaderLabels(QStringList({tr(""), tr("Name"), tr("Full Filename")}));

    for (const QString& filename : filenames)
    {
        int row = ui->tableWidget->rowCount();
        ui->tableWidget->insertRow(row);
        QTableWidgetItem* checkItem = new QTableWidgetItem();
        checkItem->setCheckState(Qt::Checked);
        ui->tableWidget->setItem(row, 0, checkItem);
        ui->tableWidget->setItem(row, 1, new QTableWidgetItem(QFileInfo(filename).fileName()));
        ui->tableWidget->setItem(row, 2, new QTableWidgetItem(filename));
    }

    ui->tableWidget->horizontalHeader()->resizeSections(QHeaderView::ResizeToContents);
    ui->tableWidget->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
    ui->tableWidget->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    ui->tableWidget->setSelectionMode(QAbstractItemView::NoSelection);
    ui->tableWidget->setSortingEnabled(true);
}

QStringList CopyIndexedFilesDialog::sources() const
{
    QStringList sources;
    for (int i = 0; i < ui->tableWidget->rowCount(); ++i)
    {
        if (ui->tableWidget->item(i, 0)->checkState() == Qt::Checked)
        {
            sources << ui->tableWidget->item(i, 2)->text();
        }
    }
    return sources;
}

void CopyIndexedFilesDialog::select(SelectMode mode)
{
    for (int i = 0; i < ui->tableWidget->rowCount(); ++i)
    {
        switch (mode)
        {
        case All:
            ui->tableWidget->item(i, 0)->setCheckState(Qt::Checked);
            break;
        case None:
            ui->tableWidget->item(i, 0)->setCheckState(Qt::Unchecked);
            break;
        case Toggle:
            switch (ui->tableWidget->item(i, 0)->checkState())
            {
            case Qt::Checked:
                ui->tableWidget->item(i, 0)->setCheckState(Qt::Unchecked);
                break;
            case Qt::Unchecked:
            default:
                ui->tableWidget->item(i, 0)->setCheckState(Qt::Checked);
                break;
            }
            break;
        }
    }
}
