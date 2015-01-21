#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "repetoire.h"
#include "calendar.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

}

MainWindow::~MainWindow()
{
    delete ui;
}
