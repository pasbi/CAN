#include "mergetreeview.h"
#include "ui_mergetreeview.h"
#include <QComboBox>
#include "mergetreeitem.h"
#include "global.h"

MergeTreeView::MergeTreeView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MergeTreeView),
    m_root(nullptr)
{
    ui->setupUi(this);
}

MergeTreeView::~MergeTreeView()
{
    delete ui;
}

QWidget* createCellWidget(MergeTreeItemBase* item)
{
    MergeInfoBase mergeInfo = *item->mergeInfoBase();
    QComboBox* box = new QComboBox();

    if (mergeInfo.type() == MergeInfoBase::AddItemType)
    {
        box->addItems( { QWidget::tr("Add to master project"),
                         QWidget::tr("Do not add to master project") } );
    }
    else if (mergeInfo.type() == MergeInfoBase::DeleteItemType)
    {
        box->addItems( { QWidget::tr("Keep in master project"),
                         QWidget::tr("Remove from master project") } );
    }

    auto on_indexChanged = [item](int index)
    {
        if (index == 0)
        {
            item->setAction(MergeInfoBase::AddItemAction);
        }
        else
        {
            item->setAction(MergeInfoBase::DeleteItemAction);
        }
    };
    QObject::connect(box, static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged), on_indexChanged);
    on_indexChanged(box->currentIndex());

    return box;
}

void MergeTreeView::setRootItem(QTreeWidgetItem *root)
{
    ui->treeWidget->clear();
    ui->treeWidget->addTopLevelItem(root);
    ui->treeWidget->expandItem(root);


    for (int i = 0; i < root->childCount(); ++i)
    {
        MergeTreeItemBase* item = static_cast<MergeTreeItemBase*>(root->child(i));
        ui->treeWidget->setItemWidget(item, 1, createCellWidget(item));
    }
    m_root = root;
}

void MergeTreeView::updateHeaderSize()
{
    int width = ui->treeWidget->viewport()->size().width();
    width -= 250;
    ui->treeWidget->header()->resizeSection(0, width);
}

void MergeTreeView::resizeEvent(QResizeEvent *e)
{
    QWidget::resizeEvent(e);
    updateHeaderSize();
}

void MergeTreeView::showEvent(QShowEvent *e)
{
    QWidget::showEvent(e);
    updateHeaderSize();
}
