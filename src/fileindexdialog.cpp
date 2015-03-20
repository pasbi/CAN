#include "fileindexdialog.h"
#include "ui_fileindexdialog.h"

FileIndexDialog::FileIndexDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FileIndexDialog)
{
    ui->setupUi(this);
}

FileIndexDialog::~FileIndexDialog()
{
    delete ui;
}
