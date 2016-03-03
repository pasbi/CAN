#include "mergedialog.h"
#include "ui_mergedialog.h"
#include "Database/SongDatabase/songdatabase.h"
#include "Database/EventDatabase/eventdatabase.h"
#include "mergeitem.h"

const QStringList MergeDialog::BUTTON_TEXT = QStringList({ MergeDialog::tr(""),
                                                           MergeDialog::tr("&Songs"),
                                                           MergeDialog::tr("&Events"),
                                                           MergeDialog::tr("&Ok") });

MergeDialog::MergeDialog(Merge *merge, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MergeDialog)
{
    ui->setupUi(this);
    setMerger(merge);
    ui->stackedWidget->setCurrentIndex(0);
    updateButtonText();
}

MergeDialog::~MergeDialog()
{
    delete ui;
}

void MergeDialog::setMerger(Merge *merger)
{
    m_merger = merger;

    ui->songWidget->setDatabaseMerger( merger->songDatabaseMerger() );
    ui->eventWidget->setDatabaseMerger( merger->eventDatabaseMerger() );
}

void MergeDialog::on_buttonNext_clicked()
{
    int i = ui->stackedWidget->currentIndex();
    int n = ui->stackedWidget->count();
    if (i == n-1) // we're on last page
    {
        accept();
    }
    else
    {
        ui->stackedWidget->setCurrentIndex(i + 1);
    }

    updateButtonText();
}

void MergeDialog::on_buttonPrevious_clicked()
{
    int i = ui->stackedWidget->currentIndex();
    ui->stackedWidget->setCurrentIndex(i - 1);
    updateButtonText();
}

void MergeDialog::on_buttonCancel_clicked()
{
    reject();
}

int MergeDialog::buttonWidth()
{
    int width = 0;
    for (const QString& text : MergeDialog::BUTTON_TEXT)
    {
        width = qMax(fontMetrics().boundingRect(text).width(), width);
    }
    return width;
}

void MergeDialog::updateButtonText()
{
    int i = ui->stackedWidget->currentIndex();
    int buttonWidth = MergeDialog::buttonWidth() + 50;
    ui->buttonPrevious->setText(BUTTON_TEXT[i]);
    ui->buttonPrevious->setFixedWidth(buttonWidth);
    ui->buttonPrevious->setEnabled(i > 0);
    ui->buttonNext->setText(BUTTON_TEXT[i + 2]);
    ui->buttonNext->setFixedWidth(buttonWidth);
    ui->buttonCancel->setFixedWidth(buttonWidth);
}
