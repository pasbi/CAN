#include "pushdialog.h"
#include "ui_pushdialog.h"

PushDialog::PushDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PushDialog)
{
    ui->setupUi(this);
}

PushDialog::~PushDialog()
{
    delete ui;
}
