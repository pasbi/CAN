#include "combinedatabaseitemsdialog.h"
#include "ui_combinedatabaseitemsdialog.h"
#include "combinedialogselectionmodel.h"
#include "mergeitem.h"
#include "Database/databaseitem.h"

CombineDatabaseItemsDialog::CombineDatabaseItemsDialog(DatabaseMergerBase* merger, MergeItem* mergeItem, QWidget* parent) :
    QDialog(parent),
    ui(new Ui::CombineDatabaseItemsDialog),
    m_mergeItem(mergeItem)
{
    ui->setupUi(this);

    Q_ASSERT(mergeItem->origin() == MergeItem::Both);

    ui->attributesTable->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Fixed);
    ui->attributesTable->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Fixed);

    ui->attributesTable->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->attributesTable->setSelectionBehavior(QAbstractItemView::SelectItems);
    ui->attributesTable->selectionModel()->deleteLater();
    ui->attributesTable->setSelectionModel(new CombineDialogSelectionModel(ui->attributesTable->model()));

    initItems();

    connect(ui->attributesTable, SIGNAL(itemClicked(QTableWidgetItem*)), this, SLOT(onSelectionChange()));

    ui->attributesTable->verticalHeader()->hide();

    DatabaseMergerBase* childMerger =  merger->childDatabaseMerger(mergeItem->master(), mergeItem->slave());
    Q_ASSERT(childMerger);
    ui->detailsTable->setDatabaseMerger( childMerger );
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

        ui->attributesTable->insertRow(row);
        QTableWidgetItem* masterItem = newTableWidgetItem(masterDisplay);
        QTableWidgetItem* slaveItem =  newTableWidgetItem(slaveDisplay);
        ui->attributesTable->setItem(row, 0, masterItem);
        ui->attributesTable->setItem(row, 1, slaveItem);

        masterItem->setSelected(modifyDetail.decision() == MergeItem::UseMaster);
        slaveItem->setSelected(modifyDetail.decision() == MergeItem::UseSlave);

        m_modifyDetails << modifyDetail;

        row++;
    }
}

void CombineDatabaseItemsDialog::updateHeaderWidths()
{
    int viewportWidth = ui->attributesTable->viewport()->width();

    ui->attributesTable->horizontalHeader()->resizeSection(0, viewportWidth/2);
    ui->attributesTable->horizontalHeader()->resizeSection(1, viewportWidth - viewportWidth/2);
    ui->attributesTable->setHorizontalHeaderItem(0, new QTableWidgetItem(tr("Master")));
    ui->attributesTable->setHorizontalHeaderItem(1, new QTableWidgetItem(tr("Other")));
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
    for (int row = 0; row < ui->attributesTable->rowCount(); ++row)
    {
        if (ui->attributesTable->item(row, 0)->isSelected())
        {
            m_modifyDetails[row].setDecision(MergeItem::UseMaster);
        }
        else
        {
            m_modifyDetails[row].setDecision(MergeItem::UseSlave);
        }
    }
}

MergeListWidget* CombineDatabaseItemsDialog::detailsMergeListWidget() const
{
    return ui->detailsTable;
}
