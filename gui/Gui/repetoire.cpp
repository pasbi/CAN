#include "repetoire.h"
#include "ui_repetoire.h"

Repetoire::Repetoire(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Repetoire)
{
    ui->setupUi(this);
}

Repetoire::~Repetoire()
{
    delete ui;
}
