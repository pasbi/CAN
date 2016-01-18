#include "datetimedialog.h"
#include "ui_datetimedialog.h"
#include "global.h"

DateTimeDialog::DateTimeDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DateTimeDialog)
{
    ui->setupUi(this);
}

DateTimeDialog::~DateTimeDialog()
{
    delete ui;
}

QDateTime DateTimeDialog::dateTime() const
{
    return ui->dateTimeEdit->dateTime();
}

void DateTimeDialog::setDateTime(const QDateTime& dateTime)
{
    ui->dateTimeEdit->setDateTime( dateTime );
}
