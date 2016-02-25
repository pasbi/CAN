#include "mergelistview.h"
#include "ui_mergelistview.h"
#include <QComboBox>
#include "mergeitem.h"
#include "global.h"

MergeListView::MergeListView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MergeListView)
{
    ui->setupUi(this);
}

MergeListView::~MergeListView()
{
    delete ui;
}

void MergeListView::setItems(const QList<MergeItemBase> items)
{
    ui->mergeListWidget->setItems(items);
}
