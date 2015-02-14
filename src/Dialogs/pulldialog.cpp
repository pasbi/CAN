#include "pulldialog.h"
#include "ui_pulldialog.h"

PullDialog::PullDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PullDialog)
{
    ui->setupUi(this);
}

PullDialog::~PullDialog()
{
    delete ui;
}
