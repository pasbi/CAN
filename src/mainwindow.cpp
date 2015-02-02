#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "util.h"
#include <QTimer>
#include <QToolButton>


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

    setupAttachmentMenu();

    // load stylesheet
    QFile styleSheetFile(":/style/style.css");
    assert( styleSheetFile.open(QIODevice::ReadOnly) );
    setStyleSheet( styleSheetFile.readAll() );
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

Song* MainWindow::currentSong() const
{
    return ui->songDatabaseWidget->currentSong();
}

#include "Commands/SongCommands/songaddattachmentcommand.h"
void MainWindow::setupAttachmentMenu()
{
    ui->actionNew_Attachment->setMenu( new QMenu( this ) );

    // toolbuttons cannot be added with desinger and popup menues require toolbuttons.
    // -> create a placeholder action in desinger and replace it with a toolbutton.
    QToolButton* toolButton = new QToolButton( this );
    toolButton->setDefaultAction( ui->actionNew_Attachment );
    ui->toolBar->insertWidget( ui->actionNew_Attachment, toolButton );
    ui->toolBar->removeAction( ui->actionNew_Attachment );

    // gather attachment creators
    for (const QString & classname : Creatable::classnamesInCategory("Attachment"))
    {
        Util::addAction( ui->actionNew_Attachment->menu(),
                         QString(tr("New %1")).arg(classname),
                         [this, classname]()
                         {
                             Song* song = currentSong();
                             if (song)
                             {
                                 SongAddAttachmentCommand* command = new SongAddAttachmentCommand( song, classname );
                                 m_project.pushCommand( command );
                                 ui->songDatabaseWidget->attachmentChooser()->update( command->attachment() );
                             }
                         } );
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
