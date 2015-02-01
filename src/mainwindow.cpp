#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "util.h"
#include <QTimer>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->stackedWidget->setCurrentIndex( 0 );

    connect( ui->buttonSongDatabase, &QPushButton::clicked, [this]()
    {
        ui->stackedWidget->setCurrentIndex( 0 );
    });

    connect( ui->buttonDateDatabase, &QPushButton::clicked, [this]()
    {
        ui->stackedWidget->setCurrentIndex( 1 );
    });


    menuBar()->setNativeMenuBar(false);
    ui->songDatabaseWidget->setSongDatabase( m_project.songDatabase() );

    connect( &m_project, SIGNAL(canUndoChanged(bool)), ui->actionUndo, SLOT(setEnabled(bool)));
    connect( &m_project, SIGNAL(canRedoChanged(bool)), ui->actionRedo, SLOT(setEnabled(bool)));


    // splitter
    connect( ui->splitter, SIGNAL(splitterMoved(int,int)), this, SLOT(resizeSplitter()));
    QTimer::singleShot(0, this, SLOT(resizeSplitter()));

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::resizeSplitter()
{
    if (ui->splitter->sizes()[0] != 0)
    {
        ui->splitter->setSizes(QList<int>() << 1 << 10000000);
    }
}












#include "Commands/SongDatabaseCommands/songdatabasenewsongcommand.h"
void MainWindow::on_actionNew_Song_triggered()
{
    m_project.pushCommand( new SongDatabaseNewSongCommand( m_project.songDatabase() ) );
}

void MainWindow::on_actionUndo_triggered()
{
    m_project.undo();
}

void MainWindow::on_actionRedo_triggered()
{
    m_project.redo();
}
