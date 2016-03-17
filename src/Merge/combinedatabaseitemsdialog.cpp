#include "combinedatabaseitemsdialog.h"

#include "ui_combinedatabaseitemsdialog.h"
#include "combinedialogselectionmodel.h"
#include "Database/databaseitembase.h"
#include <QTimer>

CombineDatabaseItemsDialog::CombineDatabaseItemsDialog(DatabaseMergerBase *merger, MergeItem *mergeItem, QWidget *parent) :
    QDialog(parent),
    ui( new Ui::CombineDatabaseItemsDialog ),
    m_mergeItem(mergeItem),
    m_databaseMerger(merger)
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
    ui->attributesTable->setHorizontalHeaderItem(0, new QTableWidgetItem(tr("Master")));
    ui->attributesTable->setHorizontalHeaderItem(1, new QTableWidgetItem(tr("Other")));
    ui->attributesTable->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    QTimer::singleShot(0, this, SIGNAL(updateHeaderWidths()));

    ui->tabWidget->setCurrentIndex(0);
}

CombineDatabaseItemsDialog::~CombineDatabaseItemsDialog()
{

}


void CombineDatabaseItemsDialog::updateHeaderWidths()
{
    int viewportWidth = ui->attributesTable->viewport()->width();

    ui->attributesTable->horizontalHeader()->resizeSection(0, viewportWidth/2);
    ui->attributesTable->horizontalHeader()->resizeSection(1, viewportWidth - viewportWidth/2);
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

QWidget* CombineDatabaseItemsDialog::detailsPage() const
{
    return ui->tabWidget->widget(1);
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

        // insert a copy. This allows us to neglect/apply the changes (if user presses cancel/Ok)
        m_modifyDetails << modifyDetail;

        row++;
    }
}

DatabaseMergerBase* CombineDatabaseItemsDialog::databaseMerger() const
{
    return m_databaseMerger;
}

void CombineDatabaseItemsDialog::setDetailsPage(QWidget* page, const QString &title)
{
    ui->tabWidget->widget(1)->layout()->addWidget(page);
    ui->tabWidget->setTabText(1, title);
}

MergeItem* CombineDatabaseItemsDialog::mergeItem() const
{
    return m_mergeItem;
}
