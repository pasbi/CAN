#include "mergedialog.h"
#include "ui_mergedialog.h"
#include "Merge/songmergewidget.h"
#include "Merge/eventmergewidget.h"
#include "Database/SongDatabase/songdatabase.h"
#include "Database/EventDatabase/eventdatabase.h"
#include "mergeitem.h"

MergeDialog::MergeDialog(Merge *merge, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MergeDialog)
{
    ui->setupUi(this);
    ui->stackedWidget->setCurrentIndex(0);
    setMerger(merge);
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

void MergeDialog::setMerger(Merge *merger)
{
    m_merger = merger;

    ui->songListWidget->setMerge(m_merger);
    ui->eventListWidget->setMerge(m_merger);
}

void MergeDialog::on_buttonCancel_clicked()
{
    reject();
}
