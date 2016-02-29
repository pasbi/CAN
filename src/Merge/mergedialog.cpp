#include "mergedialog.h"
#include "ui_mergedialog.h"
#include "Database/SongDatabase/songdatabase.h"
#include "Database/EventDatabase/eventdatabase.h"
#include "mergeitem.h"

MergeDialog::MergeDialog(Merge *merge, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MergeDialog)
{
    ui->setupUi(this);
    setMerger(merge);
}

MergeDialog::~MergeDialog()
{
    delete ui;
}

void MergeDialog::setMerger(Merge *merger)
{
    m_merger = merger;

    ui->songEventListWidget->setMerge(m_merger);
}
