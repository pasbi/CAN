#include "combinesongsdialog.h"
#include "ui_combinesongsdialog.h"
#include "Database/SongDatabase/song.h"
#include "Attachments/attachment.h"
#include "combinedialogselectionmodel.h"

CombineSongsDialog::CombineSongsDialog(const Song *masterSong, const Song *slaveSong, QWidget *parent) :
    QDialog(parent),
    CombineItems(masterSong, slaveSong),
    ui(new Ui::CombineSongsDialog)
{
    ui->setupUi(this);

    Q_ASSERT(masterSong != slaveSong);

    ui->tableWidget->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Fixed);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Fixed);

    ui->tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectItems);
    ui->tableWidget->selectionModel()->deleteLater();
    ui->tableWidget->setSelectionModel(new CombineDialogSelectionModel(ui->tableWidget->model()));

    init();

    // ui->tableWidget->selectLastColumn() does not work
    int lastColumn = ui->tableWidget->columnCount() - 1;
    for (int row = 0; row < ui->tableWidget->rowCount(); ++row)
    {
        QModelIndex index = ui->tableWidget->model()->index(row, lastColumn);
        ui->tableWidget->selectionModel()->select(index, QItemSelectionModel::Select);
    }
}

CombineSongsDialog::~CombineSongsDialog()
{
    delete ui;
}

void CombineSongsDialog::assembleCombination(Song *result)
{
    result->setTitle(m_title);
    result->setArtist(m_artist);
    result->setlabel(m_label);
    result->setState(m_state);
    result->setKey(m_key);
    result->setSingers(m_singers);
    result->setSoloPlayers(m_soloPlayers);
    result->setComments(m_comments);
}

void CombineSongsDialog::addTableWidgetItem(const QString& master, const QString& slave)
{
    int row = ui->tableWidget->rowCount();
    QTableWidgetItem* mi = new QTableWidgetItem();
    QTableWidgetItem* si = new QTableWidgetItem();
    mi->setFlags(mi->flags() & ~Qt::ItemIsEditable);
    si->setFlags(si->flags() & ~Qt::ItemIsEditable);

    mi->setFlags(mi->flags() | Qt::ItemIsSelectable);
    si->setFlags(si->flags() | Qt::ItemIsSelectable);

    mi->setText( master );
    si->setText( slave );

    ui->tableWidget->insertRow(row);
    ui->tableWidget->setItem(row, 0, mi);
    ui->tableWidget->setItem(row, 1, si);
}

void CombineSongsDialog::init()
{
    if (master()->title() != slave()->title())
    {
        addTableWidgetItem(master()->title(), slave()->title());
    }

    if (master()->artist() != slave()->artist())
    {
        addTableWidgetItem(master()->artist(), slave()->artist());
    }

    if (master()->label() != slave()->label())
    {
        addTableWidgetItem(master()->labelDisplay(), slave()->labelDisplay());
    }

    if (master()->state() != slave()->state())
    {
        addTableWidgetItem(master()->stateDisplay(), slave()->stateDisplay());
    }

    if (master()->key() != slave()->key())
    {
        addTableWidgetItem(master()->key().key(), slave()->key().key());
    }

    if (master()->singers() != slave()->singers())
    {
        addTableWidgetItem(master()->singersDisplay(), slave()->singersDisplay());
    }

    if (master()->soloPlayers() != slave()->soloPlayers())
    {
        addTableWidgetItem(master()->soloPlayersDisplay(), slave()->soloPlayersDisplay());
    }

    if (master()->comments() != slave()->comments())
    {
        addTableWidgetItem(master()->comments(), slave()->comments());
    }
}

void CombineSongsDialog::updateHeaderWidths()
{
    int viewportWidth = ui->tableWidget->viewport()->width();

    ui->tableWidget->horizontalHeader()->resizeSection(0, viewportWidth/2);
    ui->tableWidget->horizontalHeader()->resizeSection(1, viewportWidth - viewportWidth/2);
}

void CombineSongsDialog::showEvent(QShowEvent *e)
{
    QDialog::showEvent(e);
    updateHeaderWidths();
}

void CombineSongsDialog::resizeEvent(QResizeEvent *e)
{
    QDialog::resizeEvent(e);
    updateHeaderWidths();
}
