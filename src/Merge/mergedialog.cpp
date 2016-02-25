#include "mergedialog.h"
#include "ui_mergedialog.h"
#include "Merge/songmergewidget.h"
#include "Merge/eventmergewidget.h"
#include "Database/SongDatabase/songdatabase.h"
#include "Database/EventDatabase/eventdatabase.h"
#include "mergeitem.h"

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

QList<MergeItemBase> MergeDialog::songMergeItems() const
{
    return songMergeWidget()->mergeItems();
}

QList<MergeItemBase> MergeDialog::eventMergeItems() const
{
    return eventMergeWidget()->mergeItems();
}


void MergeDialog::on_buttonCancel_clicked()
{
    reject();
}
