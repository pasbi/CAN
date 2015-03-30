#include "setlistwidget.h"
#include "ui_setlistwidget.h"

SetListWidget::SetListWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SetListWidget)
{
    ui->setupUi(this);
}

SetListWidget::~SetListWidget()
{
    delete ui;
}
