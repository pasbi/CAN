#include "calendar.h"
#include "ui_calendar.h"

Calendar::Calendar(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Calendar)
{
    ui->setupUi(this);
}

Calendar::~Calendar()
{
    delete ui;
}
