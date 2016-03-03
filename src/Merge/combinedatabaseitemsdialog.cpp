#include "combinedatabaseitemsdialog.h"
#include "ui_combinesongsdialog.h"
#include "combinedialogselectionmodel.h"
#include "mergeitem.h"
#include "Database/databaseitem.h"

CombineDatabaseItemsDialog::CombineDatabaseItemsDialog(MergeItem* mergeItem, QWidget* parent) :
    QDialog(parent),
    ui(new Ui::CombineSongsDialog),
    m_mergeItem(mergeItem)
{
    ui->setupUi(this);

    Q_ASSERT(mergeItem->type() == MergeItem::Modify);

    ui->tableWidget->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Fixed);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Fixed);

    ui->tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectItems);
    ui->tableWidget->selectionModel()->deleteLater();
    ui->tableWidget->setSelectionModel(new CombineDialogSelectionModel(ui->tableWidget->model()));

    initItems();

    connect(ui->tableWidget, SIGNAL(itemClicked(QTableWidgetItem*)), this, SLOT(onSelectionChange()));

    ui->tableWidget->verticalHeader()->hide();
}

CombineDatabaseItemsDialog::~CombineDatabaseItemsDialog()
{
    delete ui;
}

QTableWidgetItem* newTableWidgetItem(const QString& label)
{
    QTableWidgetItem* item = new QTableWidgetItem();
    item->setFlags(item->flags() & ~Qt::ItemIsEditable);
    item->setFlags(item->flags() | Qt::ItemIsSelectable);
    item->setText(label);
    return item;
}

void CombineDatabaseItemsDialog::initItems()
{
    DatabaseItemBase* master = m_mergeItem->master();
    DatabaseItemBase* slave = m_mergeItem->slave();
    int row = 0;
    for (const MergeItem::ModifyDetail& modifyDetail : m_mergeItem->modifyDetails())
    {
        const QString key = modifyDetail.key();
        const QString masterDisplay = master->attributeDisplay(key);
        const QString slaveDisplay  = slave->attributeDisplay(key);

        ui->tableWidget->insertRow(row);
        QTableWidgetItem* masterItem = newTableWidgetItem(masterDisplay);
        QTableWidgetItem* slaveItem =  newTableWidgetItem(slaveDisplay);
        ui->tableWidget->setItem(row, 0, masterItem);
        ui->tableWidget->setItem(row, 1, slaveItem);

        masterItem->setSelected(modifyDetail.decision() == MergeItem::UseMaster);
        slaveItem->setSelected(modifyDetail.decision() == MergeItem::UseSlave);

        m_modifyDetails << modifyDetail;

        row++;
    }
}

void CombineDatabaseItemsDialog::updateHeaderWidths()
{
    int viewportWidth = ui->tableWidget->viewport()->width();

    ui->tableWidget->horizontalHeader()->resizeSection(0, viewportWidth/2);
    ui->tableWidget->horizontalHeader()->resizeSection(1, viewportWidth - viewportWidth/2);
    ui->tableWidget->setHorizontalHeaderItem(0, new QTableWidgetItem(tr("Master")));
    ui->tableWidget->setHorizontalHeaderItem(1, new QTableWidgetItem(tr("Other")));
}

void CombineDatabaseItemsDialog::showEvent(QShowEvent *e)
{
    QDialog::showEvent(e);
    updateHeaderWidths();
}

void CombineDatabaseItemsDialog::resizeEvent(QResizeEvent *e)
{
    QDialog::resizeEvent(e);
    updateHeaderWidths();
}

void CombineDatabaseItemsDialog::accept()
{
    m_mergeItem->updateModifyDetails(m_modifyDetails);
    QDialog::accept();
}

void CombineDatabaseItemsDialog::onSelectionChange()
{
    for (int row = 0; row < ui->tableWidget->rowCount(); ++row)
    {
        if (ui->tableWidget->item(row, 0)->isSelected())
        {
            m_modifyDetails[row].setDecision(MergeItem::UseMaster);
        }
        else
        {
            m_modifyDetails[row].setDecision(MergeItem::UseSlave);
        }
    }
}
