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
    return QDateTime( ui->calendarWidget->selectedDate(),
                      ui->timeEdit->time() );
}

void DateTimeDialog::setDateTime(const QDateTime& dateTime)
{
    ui->calendarWidget->setSelectedDate(dateTime.date());
    ui->timeEdit->setTime(dateTime.time());
}
