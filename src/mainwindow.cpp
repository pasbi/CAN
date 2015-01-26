#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "Commands/SongDatabaseCommands/songdatabasenewsongcommand.h"
#include "Database/SongDatabase/songdatabasesortproxy.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(testSlot1()));
    connect(ui->pushButton_2, SIGNAL(clicked()), this, SLOT(testSlot2()));
    connect(ui->pushButton_3, SIGNAL(clicked()), this, SLOT(testSlot3()));
    connect(ui->pushButton_4, SIGNAL(clicked()), this, SLOT(testSlot4()));
    connect(ui->pushButton_5, SIGNAL(clicked()), this, SLOT(testSlot5()));

    connect(&m_project, SIGNAL(canUndoChanged(bool)), ui->pushButton_2, SLOT(setEnabled(bool)));
    connect(&m_project, SIGNAL(canRedoChanged(bool)), ui->pushButton_3, SLOT(setEnabled(bool)));

    ui->widget->setModel(m_project.songDatabaseProxy());
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::testSlot1()
{
    m_project.pushCommand( new SongDatabaseNewSongCommand(m_project.songDatabase(), new Song(m_project.songDatabase())) );
}

void MainWindow::testSlot2()
{
    m_project.undo();
}


void MainWindow::testSlot3()
{
    m_project.redo();
}

void MainWindow::testSlot4()
{
    m_project.save("/home/pascal/test");
}

void MainWindow::testSlot5()
{
    m_project.load("/home/pascal/test");
}
