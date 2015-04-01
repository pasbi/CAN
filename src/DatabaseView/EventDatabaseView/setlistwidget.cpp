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

void SetListWidget::on_buttonSortUp_clicked()
{

}

void SetListWidget::on_buttonSortDown_clicked()
{

}

void SetListWidget::on_buttonDelete_clicked()
{

}

void SetListWidget::on_buttonAdd_clicked()
{

}
