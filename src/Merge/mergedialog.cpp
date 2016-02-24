#include "mergedialog.h"
#include "ui_mergedialog.h"
#include "Merge/songmergewidget.h"
#include "Merge/eventmergewidget.h"
#include "Database/SongDatabase/songdatabase.h"
#include "Database/EventDatabase/eventdatabase.h"

MergeDialog::MergeDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MergeDialog)
{
    ui->setupUi(this);
    ui->stackedWidget->setCurrentIndex(0);
}

MergeDialog::~MergeDialog()
{
    delete ui;
}

void MergeDialog::on_buttonBack_clicked()
{
    if (ui->stackedWidget->currentIndex() == 1)
    {
        ui->buttonNext->setText(tr("&Next"));
        ui->buttonBack->setEnabled(false);
        ui->stackedWidget->setCurrentIndex(0);
    }
}

void MergeDialog::on_buttonNext_clicked()
{
    if (ui->stackedWidget->currentIndex() == 0)
    {
        ui->buttonNext->setText(tr("&Finish"));
        ui->buttonBack->setEnabled(true);
        ui->stackedWidget->setCurrentIndex(1);
    }
    else if (ui->stackedWidget->currentIndex() == 1)
    {
        accept();
    }
}

SongMergeWidget* MergeDialog::songMergeWidget() const
{
    return static_cast<SongMergeWidget*>(ui->stackedWidget->widget(0));
}

EventMergeWidget* MergeDialog::eventMergeWidget() const
{
    return static_cast<EventMergeWidget*>(ui->stackedWidget->widget(1));
}

void MergeDialog::setMerger(Merge *merger)
{
    m_merger = merger;
    songMergeWidget()->setDatabase(merger->masterProject()->songDatabase(), merger->slaveProject()->songDatabase());
    eventMergeWidget()->setDatabase(merger->masterProject()->eventDatabase(), merger->slaveProject()->eventDatabase());
}

QList<MergeInfo<Song> > MergeDialog::songMergeItems() const
{
    const QTreeWidgetItem* root = songMergeWidget()->rootItem();
    // items are destroyed as soon as tree is destroyed, so they will not survive this class. Let's copy them...
    QList<MergeInfo<Song>> items;

    for (int i = 0; i < root->childCount(); ++i)
    {
        QTreeWidgetItem* item = root->child(i);
        if (item->childCount() == 0) // if the item has no children, merge on song level (otherwise on attachment level, see below (not implemented yet))
        {
            MergeTreeItemBase* mtib = static_cast<MergeTreeItemBase*>(item);
            Q_ASSERT(mtib->type() == MergeTreeItemBase::SongType);
            items << *static_cast<MergeTreeItem<Song>*>(item)->mergeInfo();
        }
    }

    return items;
}

QList<MergeInfo<Event> > MergeDialog::eventMergeItems() const
{
    const QTreeWidgetItem* root = eventMergeWidget()->rootItem();
    // items are destroyed as soon as tree is destroyed, so they will not survive this class. Let's copy them...
    QList<MergeInfo<Event>> items;

    for (int i = 0; i < root->childCount(); ++i)
    {
        QTreeWidgetItem* item = root->child(i);
        if (item->childCount() == 0) // if the item has no children, merge on song level (otherwise on attachment level, see below (not implemented yet))
        {
            MergeTreeItemBase* mtib = static_cast<MergeTreeItemBase*>(item);
            Q_ASSERT(mtib->type() == MergeTreeItemBase::EventType);
            items << *static_cast<MergeTreeItem<Event>*>(item)->mergeInfo();
        }
    }

    return items;
}


void MergeDialog::on_buttonCancel_clicked()
{
    reject();
}
