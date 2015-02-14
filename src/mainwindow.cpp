#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "util.h"
#include <QTimer>
#include <QToolButton>
#include <QMessageBox>
#include <QFileDialog>
#include <QCloseEvent>
#include <QProcess>
#include <QClipboard>
#include "application.h"
#include "Dialogs/addfileindexsourcedialog.h"


REGISTER_DEFN_CONFIG( MainWindow, "Global" );

CONFIGURABLE_ADD_ITEM( MainWindow, RecentProject, "", ConfigurationItemOptions::HiddenInterface() );

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // load configuration
    Configurable::restoreAll();

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
    ui->actionUndo->setEnabled( m_project.canUndo() );
    ui->actionRedo->setEnabled( m_project.canRedo() );


    // splitter
    connect( ui->splitter, SIGNAL(splitterMoved(int,int)), this, SLOT(resizeSplitter()));
    QTimer::singleShot(0, this, SLOT(resizeSplitter()));

    setupAttachmentMenu();


    connect( m_project.songDatabase(), &SongDatabase::attachmentAdded, [this](int i)
    {
        setCurrentAttachment( i );
    });

    connect( m_project.songDatabase(), &SongDatabase::attachmentRemoved, [this](int i)
    {
        setCurrentAttachment( i );
    });

    connect( &m_project, SIGNAL(canCloseChanged(bool)), this, SLOT(updateWindowTitle()) );
    updateWindowTitle();

    loadDefaultProject();

    ui->menu_path->setTitle( m_project.path() );
    connect(ui->actionOpen_Terminal_here, &QAction::triggered, [this]()
    {
        QProcess::startDetached( "gnome-terminal", QStringList(), m_project.path() );
    });
    connect(ui->actionCopyToClipboard, &QAction::triggered, [this]()
    {
        qApp->clipboard()->setText( m_project.path(), QClipboard::Clipboard );
        qApp->clipboard()->setText( m_project.path(), QClipboard::Selection );
    });


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
                             }
                         } );
    }
}

void MainWindow::setCurrentAttachment( int index )
{
    ui->songDatabaseWidget->attachmentChooser()->setAttachment( index );
}




//////////////////////////////////////////////////
////
///     Saving and loading stuff
//
////////////////////////////////////////////////


QString MainWindow::projectName() const
{
    QString filename = QFileInfo(m_currentPath).fileName();
    if (filename.isEmpty())
    {
        return tr("Unnamed");
    }
    else
    {
        return filename;
    }
}

bool MainWindow::saveProject()
{
    if (m_currentPath.isEmpty())
    {
        return saveProjectAs();
    }
    else
    {
        bool success = m_project.save( m_currentPath );
        if (success)
        {
            updateWindowTitle();
            return true;
        }
        else
        {
            return false;
        }
    }
}

void MainWindow::setCurrentPath(const QString &path)
{
    m_currentPath = path;
    config.setItem( "RecentProject", m_currentPath );
}

QString MainWindow::proposedPath() const
{
    QString filename = projectName();
    QString ending = QString(".%1").arg(m_project.ending());

    if (!filename.endsWith(ending))
    {
        filename = filename.append(ending);
    }

    if (m_currentPath.isEmpty())
    {
        return QDir::home().absoluteFilePath( filename );
    }
    else
    {
        return m_currentPath;
    }
}

bool MainWindow::saveProjectAs()
{
    QString filename =
    QFileDialog::getSaveFileName( this,
                                  tr("Save As ..."),
                                  proposedPath(),
                                  filter()          );
    if (filename.isEmpty())
    {
        return false;
    }
    else
    {
        setCurrentPath(filename);
        return saveProject();
    }
}

bool MainWindow::newProject()
{
    if (!canProjectClose())
    {
        return false;
    }

    m_project.reset();
    setCurrentPath( QString() );
    updateWindowTitle();
    return true;
}

bool MainWindow::openProject()
{
    if (!canProjectClose())
    {
        return false;
    }

    QString filename =
    QFileDialog::getOpenFileName( this,
                                  tr("Open ..."),
                                  proposedPath(),
                                  filter()              );
    if (filename.isEmpty())
    {
        return false;
    }
    else
    {
        setCurrentPath(filename);
        updateWindowTitle();
        return m_project.load( filename );
    }
}

bool MainWindow::canProjectClose()
{
    if (m_project.canClose())
    {
        return true;
    }
    else
    {
        switch (
        QMessageBox::question( this,
                               tr("Really close %1").arg( projectName() ),
                               tr("There are unsaved changes."),
                               QMessageBox::Save | QMessageBox::Discard | QMessageBox::Abort,
                               QMessageBox::Abort ) )
        {
        case QMessageBox::Save:
            return saveProject();
        case QMessageBox::Discard:
            return true;
        case QMessageBox::Abort:
            return false;
        default:
            WARNING << "Illegal case in switch statement";
            return false;
        }
    }
}

void MainWindow::updateWindowTitle()
{
    QString star = m_project.canClose() ? "" : "*";
    QString cloudState = "[ No Cloud ]";

    QString title = QString("%1%2 - %3 - %4")
            .arg( projectName() )
            .arg( star )
            .arg( cloudState )
            .arg( qApp->applicationName() );

    setWindowTitle( title );
}

void MainWindow::closeEvent(QCloseEvent *e)
{
    if (canProjectClose())
    {
        QMainWindow::closeEvent(e);
        e->accept();
    }
    else
    {
        e->ignore();
    }
}

void MainWindow::loadDefaultProject()
{
    m_currentPath = config.item( "RecentProject" ).toString();

    if (!m_currentPath.isEmpty())
    {
        if (QFileInfo(m_currentPath).isReadable())
        {
            if ( m_project.load( m_currentPath ) )
            {
            }
            else
            {
                QMessageBox::warning( this,
                                      QString(tr("Opening %1")).arg(m_currentPath),
                                      QString(tr("Cannot open %1. Unknown file format.")).arg(m_currentPath),
                                      QMessageBox::Ok
                                      );
                m_project.reset();
                config.setItem("RecentProject", "");
            }
        }
        else
        {
            QMessageBox::warning( this,
                                  QString(tr("Opening %1")).arg(m_currentPath),
                                  QString(tr("File %1 not found.")).arg(m_currentPath),
                                  QMessageBox::Ok
                                  );
            m_project.reset();
            setCurrentPath("");
        }
        updateWindowTitle();
    }
}



























////////////////////////////////////////////////
////
///     Connections to Actions
//
///////////////////////////////////////////////


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

#include "Commands/SongCommands/songremoveattachmentcommand.h"
void MainWindow::on_actionDelete_Attachment_triggered()
{
    Song* song = currentSong();
    int index = ui->songDatabaseWidget->attachmentChooser()->currentAttachmentIndex();

    if (song && index >= 0)
    {
        m_project.pushCommand( new SongRemoveAttachmentCommand( song, index ) );
    }
}

void MainWindow::on_actionNew_Project_triggered()
{
    newProject();
}

void MainWindow::on_actionSave_triggered()
{
    saveProject();
}

void MainWindow::on_actionSave_As_triggered()
{
    saveProjectAs();
}

void MainWindow::on_actionOpen_triggered()
{
    openProject();
}

#include "Commands/SongDatabaseCommands/songdatabaseremovesongcommand.h"
void MainWindow::on_actionDelete_Song_triggered()
{
    Song* song = currentSong();
    if (song)
    {
        m_project.pushCommand( new SongDatabaseRemoveSongCommand( m_project.songDatabase(), song ) );
    }
}

void MainWindow::on_actionUpdate_Index_triggered()
{
    app().fileIndex().updateIndex();
}

void MainWindow::on_actionAdd_Folder_triggered()
{
    AddFileIndexSourceDialog dialog;
    dialog.setDirectory( QDir::homePath() );
    dialog.setOptions( QFileDialog::ShowDirsOnly );
    dialog.setFileMode( QFileDialog::Directory );
    if (dialog.exec() != QDialog::Accepted)
    {
        return;
    }

    QStringList filter = dialog.filter();
    QString path = dialog.selectedFiles().first();
    app().fileIndex().addSource( path, filter );
}

void MainWindow::on_actionClear_Index_triggered()
{
    app().fileIndex().clear();
}
