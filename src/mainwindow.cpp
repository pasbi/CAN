#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "Commands/SongDatabaseCommands/songdatabasenewsongcommand.h"
#include "Database/SongDatabase/songdatabasesortproxy.h"
#include "Attachments/chordpatternattachment.h"
#include "Attachments/pdfattachment.h"


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
