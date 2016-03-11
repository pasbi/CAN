#include "combineattachmentsdialog.h"
#include "ui_combineattachmentsdialog.h"

#include "mergeitem.h"

CombineAttachmentsDialog::CombineAttachmentsDialog(MergeItem *mergeItem, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CombineAttachmentsDialog)
{
    ui->setupUi(this);
}

CombineAttachmentsDialog::~CombineAttachmentsDialog()
{
    delete ui;
}
