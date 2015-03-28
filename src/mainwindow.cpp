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
#include "Dialogs/stringdialog.h"
#include "DatabaseView/SongDatabaseView/songtableview.h"
#include "conflicteditor.h"
#include "Dialogs/commitdialog.h"
#include "Dialogs/identitydialog.h"
#include "Dialogs/configurationdialog.h"
#include <QProgressDialog>
#include <QScrollArea>
#include <QLabel>

DEFN_CONFIG( MainWindow, "Global" );

QString defaultStyleSheet()
{
    QFile file(":/style/styles/stylesheet.qss");

    assert( file.open(QIODevice::ReadOnly ));
    return file.readAll();
}

CONFIGURABLE_ADD_ITEM_HIDDEN( MainWindow, RecentProject, "");
CONFIGURABLE_ADD_ITEM_HIDDEN( MainWindow, FileIndexFilter, "");
CONFIGURABLE_ADD_ITEM( MainWindow, Style,               "Style",                  QVariant(),           ConfigurationItemOptions::TextEditOptions( "Stylesheet" ) );
CONFIGURABLE_ADD_ITEM( MainWindow, AskForCommitMessage, "Ask for commit message", QVariant(true),       ConfigurationItemOptions::CheckboxOptions() );
CONFIGURABLE_ADD_ITEM( MainWindow, CommitMessage,       "Commit message",         "Sync",               ConfigurationItemOptions::TextEditOptions( "commit message" ) );



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //     load stylesheet
    config.item("Style")->setDefaultValue( defaultStyleSheet() );
    connect( config.item("Style"), &ConfigurationItem::valueChanged, [this](QVariant value)
    {
        app().setStyleSheet( value.toString() );
    });
    app().setStyleSheet( config.value( "Style" ).toString() );

    //////////////////////////////////////////
    /// restore configuration
    //////////////////////////////////////////
    /// is done implicitly when Configurable is loaded


    //////////////////////////////////////////
    ///
    //////////////////////////////////////////
    connect( ui->buttonSongDatabase, &QPushButton::clicked, [this]()
    {
        ui->stackedWidget->setCurrentIndex( 0 );
    });
    connect( ui->buttonDateDatabase, &QPushButton::clicked, [this]()
    {
        ui->stackedWidget->setCurrentIndex( 1 );
    });
    ui->stackedWidget->setCurrentIndex( 0 );


    menuBar()->setNativeMenuBar(false);
    setupAttachmentMenu();
    ui->songDatabaseWidget->setSongDatabase( m_project.songDatabase() );
    ui->dateDatabaseWidget->setDateDatabase( m_project.dateDatabase() );


    //////////////////////////////////////////
    /// Undo/Redo
    //////////////////////////////////////////
    connect( &m_project, SIGNAL(canUndoChanged(bool)), ui->actionUndo, SLOT(setEnabled(bool)));
    connect( &m_project, SIGNAL(canRedoChanged(bool)), ui->actionRedo, SLOT(setEnabled(bool)));
    ui->actionUndo->setEnabled( m_project.canUndo() );
    ui->actionRedo->setEnabled( m_project.canRedo() );


    //////////////////////////////////////////
    /// Splitter
    //////////////////////////////////////////
    connect( ui->splitter, SIGNAL(splitterMoved(int,int)), this, SLOT(resizeSplitter()));
    QTimer::singleShot(0, this, SLOT(resizeSplitter()));


    //////////////////////////////////////////
    ///
    //////////////////////////////////////////
    connect( m_project.songDatabase(), &SongDatabase::attachmentAdded, [this](int i)
    {
        setCurrentAttachment( i );
    });
    connect( m_project.songDatabase(), &SongDatabase::attachmentRemoved, [this](int i)
    {
        setCurrentAttachment( i );
    });
    connect( m_project.songDatabase(), &SongDatabase::attachmentRenamed, [this](int i, QString)
    {
        setCurrentAttachment( i );
    });
    connect( m_project.songDatabase(), &SongDatabase::dataChanged, [this]()
    {
        ui->songDatabaseWidget->attachmentChooser()->updateAttachmentView();
    });

    //////////////////////////////////////////
    ///
    //////////////////////////////////////////
    connect( &m_project, SIGNAL(canCloseChanged(bool)), this, SLOT(updateWindowTitle()) );
    updateWindowTitle();
    connect( ui->songDatabaseWidget->tableView()->selectionModel(),
             &QItemSelectionModel::currentRowChanged,
             [this](){
        QTimer::singleShot(0, this, SLOT( updateWhichWidgetsAreEnabled() ));
    });
    updateWhichWidgetsAreEnabled();

    loadDefaultProject();

    m_project.setResolveConflictsCallback( [this]()
        {
            QList<File> files = m_project.conflictingFiles();
            while (!files.isEmpty())
            {
                ConflictEditor editor( files, this );
                editor.exec();
                files = m_project.conflictingFiles();
            }

        } );

    m_identityManager.restore();
}

MainWindow::~MainWindow()
{
    delete ui;
    m_identityManager.save();
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
                         QString(tr("New %1")).arg( Creatable::name(classname) ),
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
        bool success  = m_project.saveZip( m_currentPath );
//             success &= m_project.loadFromTempDir(); // files might have changed
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
    config.set( "RecentProject", m_currentPath );
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
    bool success;
    if (filename.isEmpty())
    {
        success = false;
    }
    else
    {
        setCurrentPath(filename);
        updateWindowTitle();
        success = m_project.loadZip( filename );
        m_project.loadFromTempDir();
    }


    updateWhichWidgetsAreEnabled();
    return success;
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

    QString title = QString("%1%2 - %4")
            .arg( projectName() )
            .arg( star )
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
    m_currentPath = config.value( "RecentProject" ).toString();

    if (!m_currentPath.isEmpty())
    {
        if (QFileInfo(m_currentPath).isReadable())
        {
            if ( m_project.loadZip( m_currentPath ) )
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
                config.set("RecentProject", "");
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
        updateWhichWidgetsAreEnabled();
    }
}

int MainWindow::currentAttachmentIndex() const
{
    return ui->songDatabaseWidget->attachmentChooser()->currentAttachmentIndex();
}

Attachment* MainWindow::currentAttachment() const
{
    if (currentSong() == NULL)
    {
        return NULL;
    }
    int index = currentAttachmentIndex();
    if (index < 0)
    {
        return NULL;
    }
    else
    {
        if (currentSong()->attachments().isEmpty())
        {
            return NULL;
        }
        else
        {
            return currentSong()->attachments()[index];
        }
    }

}

void setEnabled( QObject* o, bool enable )
{
    QAction* a = qobject_cast<QAction*>(o);
    if (a) a->setEnabled(enable);

    QWidget* w = qobject_cast<QWidget*>(o);
    if (w) w->setEnabled(enable);
}

void MainWindow::updateWhichWidgetsAreEnabled()
{
    Project* cProject = &m_project;
    Song* cSong = currentSong();
    Attachment* cAttachment = currentAttachment();
    bool cGit = cProject ? cProject->isGitRepository() : false;

    //TODO list dependencies
    QObjectList attachmentObjects, songObects, projectObjects, gitObjects, alwaysObjects;

    projectObjects      << ui->actionNew_Song;
    alwaysObjects       << ui->actionNew_Project;
    projectObjects      << ui->actionSave;
    projectObjects      << ui->actionSave_As;
    // ui->actionOpen;
    // ui->actionUpdate_Index;
    // ui->actionAdd_Folder;
    // ui->actionClear_Index;
    songObects          << ui->actionNew_Attachment;
    attachmentObjects   << ui->actionDelete_Attachment;
    // ui->actionUndo;
    // ui->actionRedo;
    songObects          << ui->actionDelete_Song;
    gitObjects          << ui->actionSync;
    // ui->actionClone;
    // ui->actionOpen_Terminal_here;
    attachmentObjects   << ui->actionRename_Attachment;
    attachmentObjects   << ui->actionDuplicate_Attachment;



    for (QObject* o : projectObjects )      ::setEnabled( o, !!cProject     && currentPage() == SongDatabasePage );
    for (QObject* o : songObects )          ::setEnabled( o, !!cSong        && currentPage() == SongDatabasePage );
    for (QObject* o : attachmentObjects)    ::setEnabled( o, !!cAttachment  && currentPage() == SongDatabasePage );
    for (QObject* o : gitObjects)           ::setEnabled( o, !!cGit         && currentPage() == SongDatabasePage );
}

MainWindow::Page MainWindow::currentPage() const
{
    switch (ui->stackedWidget->currentIndex())
    {
    case 0:
        return SongDatabasePage;
    case 1:
        return DateDatabasePage;
    default:
        assert( false );
        return (Page) -1;
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
    updateWhichWidgetsAreEnabled();
}

void MainWindow::on_actionUndo_triggered()
{
    m_project.undo();
    updateWhichWidgetsAreEnabled();
}

void MainWindow::on_actionRedo_triggered()
{
    m_project.redo();
    updateWhichWidgetsAreEnabled();
}

#include "Commands/SongCommands/songremoveattachmentcommand.h"
void MainWindow::on_actionDelete_Attachment_triggered()
{
    Song* song = currentSong();
    int index = ui->songDatabaseWidget->attachmentChooser()->currentAttachmentIndex();

    if (song && index >= 0)
    {
        m_project.pushCommand( new SongRemoveAttachmentCommand( song, index ) );
        updateWhichWidgetsAreEnabled();
    }
}

void MainWindow::on_actionNew_Project_triggered()
{
    newProject();
    updateWhichWidgetsAreEnabled();
}

void MainWindow::on_actionSave_triggered()
{
    saveProject();
    updateWhichWidgetsAreEnabled();
}

void MainWindow::on_actionSave_As_triggered()
{
    saveProjectAs();
    updateWhichWidgetsAreEnabled();
}

void MainWindow::on_actionOpen_triggered()
{
    openProject();
    updateWhichWidgetsAreEnabled();
}

#include "Commands/SongDatabaseCommands/songdatabaseremovesongcommand.h"
void MainWindow::on_actionDelete_Song_triggered()
{
    Song* song = currentSong();
    if (song)
    {
        m_project.pushCommand( new SongDatabaseRemoveSongCommand( m_project.songDatabase(), song ) );
        updateWhichWidgetsAreEnabled();
    }
}

void MainWindow::on_actionAdd_Folder_triggered()
{
    AddFileIndexSourceDialog dialog( config.value("FileIndexFilter").toString(), this );
    dialog.setDirectory( QDir::homePath() );
    dialog.setOptions( QFileDialog::ShowDirsOnly );
    dialog.setFileMode( QFileDialog::Directory );
    if (dialog.exec() != QDialog::Accepted)
    {
        return;
    }
    config.set("FileIndexFilter", dialog.filter() );

    QStringList filter = dialog.filterList();
    QString path = dialog.selectedFiles().first();

    QProgressDialog pd( "Task in Progress", "Cancel", 0, -1, this );
    pd.setWindowModality( Qt::WindowModal );

    app().fileIndex().addSource( path, filter );

    QLabel* label = new QLabel(&pd);
    label->setWordWrap(true);
    pd.setLabel(label);
    pd.show();

    while (!app().fileIndex().operationIsFinished())
    {
        pd.setValue( (pd.value() + 1) % 100 );
        label->setText( QString(tr("%1\n%2")).arg(app().fileIndex().currentFilename())
                                              .arg(app().fileIndex().size())            );
        qApp->processEvents();
        QThread::msleep( 10 );
        if (pd.wasCanceled())
        {
            app().fileIndex().abortOperations();
        }
    }

}

void MainWindow::on_actionClear_Index_triggered()
{
    app().fileIndex().clear();
}

#include "Commands/AttachmentCommands/attachmentrenamecommand.h"
void MainWindow::on_actionRename_Attachment_triggered()
{
    Song* cs = currentSong();
    assert( cs );

    int index = ui->songDatabaseWidget->attachmentChooser()->currentAttachmentIndex();
    assert( index >= 0 );

    Attachment* attachment = cs->attachments()[index];
    QString name = attachment->name();
    QString newName = StringDialog::getString( name, QString(tr("New Name for %1")).arg(name), this );
    if (newName.isEmpty())
    {
        return;
    }

    m_project.pushCommand( new AttachmentRenameCommand( attachment, newName ) );
    updateWhichWidgetsAreEnabled();
}

#include "Commands/SongCommands/songduplicateattachmentcommand.h"
void MainWindow::on_actionDuplicate_Attachment_triggered()
{
    Song* cs = currentSong();
    assert( cs );

    int index = ui->songDatabaseWidget->attachmentChooser()->currentAttachmentIndex();
    assert( index >= 0 );

    Attachment* attachment = cs->attachments()[index];
    assert( attachment );

    m_project.pushCommand( new SongDuplicateAttachmentCommand( attachment ) );
    updateWhichWidgetsAreEnabled();
}

void MainWindow::on_actionOpen_Terminal_here_triggered()
{
    QProcess::startDetached( "gnome-terminal", QStringList(), m_project.path() );
}

void MainWindow::on_actionClone_triggered()
{
    QFileDialog fd(this);
    fd.setWindowTitle( tr("Clone ...") );
    fd.setDirectoryUrl( QUrl::fromLocalFile( QDir::homePath() ) );
    fd.setFileMode( QFileDialog::Directory );
    fd.setOption( QFileDialog::ShowDirsOnly, true );

    if (fd.exec() != QDialog::Accepted || fd.selectedUrls().isEmpty())
    {
        return;
    }


    QUrl url = fd.selectedUrls()[0];

    if (!url.isValid())
    {
        return;
    }

    setCurrentPath("");
    if ( !m_project.clone( url.url() ))
    {
        QMessageBox::warning( this,
                              tr("Cloning failed"),
                              QString(tr("Failed to clone %1.").arg(url.url())),
                              QMessageBox::Ok,
                              QMessageBox::NoButton );
    }

    updateWindowTitle();
    updateWhichWidgetsAreEnabled();
}

void MainWindow::on_actionSync_triggered()
{
    QString message;
    Identity identity;

    if ( config.value("AskForCommitMessage").toBool() )
    {
        CommitDialog dialog(&m_identityManager, this);
        if (dialog.exec() != QDialog::Accepted)
        {
            // process aborted
            return;
        }
        else
        {
            message = dialog.message();
            identity = dialog.identity();
        }
    }
    else
    {
        identity = m_identityManager.currentIdentity();
        message = config.value( "CommitMessage" ).toString();
    }

    if ( !identity.isValid() )
    {
        QMessageBox::warning( this,
                              tr("Sync"),
                              tr("You must provide a valid identity to sync. Abort.") );
        return;
    }
    else if ( message.isEmpty() )
    {
        QMessageBox::warning( this,
                              tr("Sync"),
                              tr("Commit message may not be empty. Abort.") );
        return;
    }


    if ( m_project.sync( message, identity ) )
    {
        QMessageBox::information( this,
                                  tr("Sync"),
                                  tr("Sync succeeded"),
                                  QMessageBox::Ok,
                                  QMessageBox::NoButton );
    }
    else
    {
        QMessageBox::information( this,
                                  tr("Sync"),
                                  tr("Sync failed."),
                                  QMessageBox::Ok,
                                  QMessageBox::NoButton );
    }
}

void MainWindow::on_actionIdentites_triggered()
{
    IdentityDialog dialog( &m_identityManager, this );
    dialog.exec();
}

void MainWindow::on_actionSettings_triggered()
{
    ConfigurationDialog dialog(this);
    dialog.exec();
}

void MainWindow::on_action_Index_Info_triggered()
{
    QMessageBox::information( this,
                              tr("File index information"),
                              QString(tr("Files: %1")).arg(app().fileIndex().size()) );
}




























