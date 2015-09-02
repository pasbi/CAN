#include "mainwindow.h"

#include <QTimer>
#include <QToolButton>
#include <QMessageBox>
#include <QFileDialog>
#include <QCloseEvent>
#include <QProcess>
#include <QClipboard>
#include <QProgressDialog>
#include <QScrollArea>
#include <QLabel>
#include <QThread>

#include "ui_mainwindow.h"
#include "util.h"
#include "application.h"
#include "Dialogs/addfilestoindexdialog.h"
#include "Dialogs/stringdialog.h"
#include "Dialogs/commitdialog.h"
#include "Dialogs/clonedialog.h"
#include "Dialogs/identitydialog.h"
#include "Dialogs/configurationdialog.h"
#include "Dialogs/programdialog.h"
#include "Dialogs/tagdialog.h"
#include "conflicteditor.h"
#include "Commands/DatabaseCommands/databasenewitemcommand.h"
#include "Commands/DatabaseCommands/databaseedititemcommand.h"
#include "Commands/DatabaseCommands/databaseremoveitemcommand.h"
#include "Commands/SongDatabaseCommands/songdatabasesetcolumnvisibilitycommand.h"
#include "Commands/SongCommands/songremoveattachmentcommand.h"
#include "Commands/SongCommands/songnewattachmentcommand.h"
#include "Commands/edittagscommand.h"
#include "Commands/AttachmentCommands/attachmentrenamecommand.h"
#include "Commands/SongCommands/songeditprogramcommand.h"
#include "AttachmentView/attachmentchooser.h"
#include "Attachments/attachment.h"
#include "Database/EventDatabase/setlistitem.h"
#include "Database/SongDatabase/songdatabase.h"
#include "Database/EventDatabase/eventdatabase.h"
#include "DatabaseView/EventDatabaseView/eventtableview.h"
#include "DatabaseView/SongDatabaseView/songtableview.h"
#include "creatable.h"
#include "DatabaseView/SongDatabaseView/songtableview.h"
#include "Project/identity.h"
#include "PDFCreator/orphantsetlist.h"
#include "PDFCreator/pdfcreator.h"

DEFN_CONFIG( MainWindow, "Global" );

QString defaultStyleSheet()
{
    QFile file(":/style/styles/stylesheet.qss");

    assert( file.open(QIODevice::ReadOnly ));
    return file.readAll();
}

CONFIGURABLE_ADD_ITEM_HIDDEN( MainWindow, RecentProject, "");
CONFIGURABLE_ADD_ITEM_HIDDEN( MainWindow, RecentCloneURL, QDir::homePath());
CONFIGURABLE_ADD_ITEM_HIDDEN( MainWindow, FileIndexFilter, "");
CONFIGURABLE_ADD_ITEM_HIDDEN( MainWindow, locale, QLocale::system().name().toStdString().c_str());
CONFIGURABLE_ADD_ITEM( MainWindow,
                       Style,
                       QT_TRANSLATE_NOOP("ConfigurableItem", "Style"),
                       QT_TRANSLATE_NOOP("ConfigurableItem", "ConfigurableItem/MainWindow/Style"),
                       QVariant(),  // is set in MainWindow constructor since ::defaultStyleSheet() is not available in static context.
                       ConfigurableItemOptions::TextEditOptions( QT_TRANSLATE_NOOP( "ConfigurableItem", "Stylesheet") )
                       );
CONFIGURABLE_ADD_ITEM( MainWindow,
                       AskForCommitMessage,
                       QT_TRANSLATE_NOOP("ConfigurableItem", "Ask for commit message"),
                       QT_TRANSLATE_NOOP("ConfigurableItem", "ConfigurableItem/MainWindow/AskForCommitMessage"),
                       QVariant(true),
                       ConfigurableItemOptions::CheckboxOptions()
                       );
CONFIGURABLE_ADD_ITEM( MainWindow,
                       CommitMessage,
                       QT_TRANSLATE_NOOP("ConfigurableItem", "Commit message"),
                       QT_TRANSLATE_NOOP("ConfigurableItem", "ConfigurableItem/MainWindow/CommitMessage"),
                       QT_TRANSLATE_NOOP("ConfigurableItem", "Synchronizing"),
                       ConfigurableItemOptions::TextEditOptions( QT_TRANSLATE_NOOP( "ConfigurableItem", "commit message") )
                       );
CONFIGURABLE_ADD_ITEM( MainWindow,
                       AskForSynchronizing,
                       QT_TRANSLATE_NOOP("ConfigurableItem", "Remind synchronizing"),
                       QT_TRANSLATE_NOOP("ConfigurableItem", "ConfigurableItem/MainWindow/AskForSynchronizing"),
                       true,
                       ConfigurableItemOptions::CheckboxOptions()
                       );

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    setWindowIcon( QIcon(":/icons/icons/01-elephant-icon.png") );
    app().setMainWindow( this );
    app().setProject( &m_project );
    createAttachmentActions();

    ui->setupUi(this);
    createDebugMenu();

    QSettings settings;
    restoreGeometry( settings.value("Geometry").toByteArray() );

    //     load stylesheet
    config.item("Style")->setDefaultValue( defaultStyleSheet() );
    connect( config.item("Style"), &ConfigurableItem::valueChanged, [this](QVariant value)
    {
        app().setStyleSheet( value.toString() );
    });
    app().setStyleSheet( config.value( "Style" ).toString() );

    //////////////////////////////////////////
    /// restore Configurable
    //////////////////////////////////////////
    /// is done implicitly when Configurable is loaded


    menuBar()->setNativeMenuBar(false);
    setupAttachmentMenu();

    //////////////////////////////////////////
    /// Undo/Redo
    //////////////////////////////////////////
    connect( &m_project, SIGNAL(canUndoChanged(bool)), ui->actionUndo, SLOT(setEnabled(bool)));
    connect( &m_project, SIGNAL(canRedoChanged(bool)), ui->actionRedo, SLOT(setEnabled(bool)));

    auto action = [this]()
    {
        const QUndoCommand* nextUndoCommand = m_project.command( m_project.QUndoStack::index() - 1);
        const QUndoCommand* nextRedoCommand = m_project.command( m_project.QUndoStack::index() );

        QString undoText = tr("&Undo %1").arg( nextUndoCommand ? nextUndoCommand->actionText() : "" );
        QString redoText = tr("&Redo %1").arg( nextRedoCommand ? nextRedoCommand->actionText() : "" );

        undoAction()->setText( undoText );
        redoAction()->setText( redoText );
    };

    connect( &m_project, &Project::commandPushed, action );
    connect( &m_project, &Project::undoStackCleared, [this]()
    {
        undoAction()->setText( tr("&Undo") );
        redoAction()->setText( tr("&Redo") );
    });
    undoAction()->setText( tr("&Undo") );
    redoAction()->setText( tr("&Redo") );
    ui->actionUndo->setEnabled( m_project.canUndo() );
    ui->actionRedo->setEnabled( m_project.canRedo() );

    // very important to set associated widget. Else, shortcuts would be ambigous.
    initAction( actionNew_Song,         ui->songDatabaseWidget->songTableView(),    tr("&New Song"),       tr("Add a new song."),        "Ctrl+N",   ui->menuSongs,  "" )
    initAction( actionDelete_Song,      ui->songDatabaseWidget->songTableView(),    tr("&Remove Song"),    tr("Remove selected song."),  "Del",      ui->menuSongs,  ":/icons/icons/rubbish7.png" )
    initAction( actionCopy_Song,        ui->songDatabaseWidget->songTableView(),    tr("&Copy Song"),      tr("Copy selected song."),    "Ctrl+C",   ui->menuSongs,  "" )
    initAction( actionPaste_Song,       ui->songDatabaseWidget->songTableView(),    tr("&Paste Song"),     tr("Paste song."),            "Ctrl+V",   ui->menuSongs,  "" )
    initAction( actionEdit_Program,     ui->songDatabaseWidget->songTableView(),    tr("&Edit Program"),   tr("Edit program."),          "",         ui->menuSongs,  "" )
    initAction( actionEdit_Song_Tags,   ui->songDatabaseWidget->songTableView(),    tr("&Edit Tags"),      tr("Edit tags of the song."), "",         ui->menuSongs,  ":/icons/icons/tag-2.png" )

    initAction( actionNew_Event,        ui->eventDatabaseWidget->eventTableView(),  tr("&New Event"),      tr("Add a new event."),       "Ctrl+N",   ui->menuEvents, "" )
    initAction( actionDelete_Event,     ui->eventDatabaseWidget->eventTableView(),  tr("&Remove Event"),   tr("Remove selected event."), "Del",      ui->menuEvents, ":/icons/icons/rubbish7.png" )
    initAction( actionCopy_Event,       ui->eventDatabaseWidget->eventTableView(),  tr("&Copy Event"),     tr("Copy selected event."),   "Ctrl+C",   ui->menuEvents, "" )
    initAction( actionPaste_Event,      ui->eventDatabaseWidget->eventTableView(),  tr("&Paste Event"),    tr("Paste event."),           "Ctrl+V",   ui->menuEvents, "" )
    initAction( actionEdit_Event_Tags,  ui->eventDatabaseWidget->eventTableView(),  tr("&Edit Tags"),      tr("Edit tags of the event."),"",         ui->menuEvents, ":/icons/icons/tag-2.png" )


    //////////////////////////////////////////
    /// Splitter
    //////////////////////////////////////////
    connect( ui->splitter, SIGNAL(splitterMoved(int,int)), this, SLOT(resizeSplitter()));
    QTimer::singleShot(10, this, SLOT(resizeSplitter()));


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
    connect( ui->songDatabaseWidget->songTableView()->selectionModel(),
             &QItemSelectionModel::currentRowChanged,
             [this](){
        QTimer::singleShot(0, this, SLOT( updateActionsEnabled() ));
    });
    connect( ui->eventDatabaseWidget->eventTableView()->selectionModel(),
             &QItemSelectionModel::currentRowChanged,
             [this](){
        QTimer::singleShot(0, this, SLOT( updateActionsEnabled() ));
    });
    updateActionsEnabled();

    loadDefaultProject();

    m_identityManager.restore();

    connect( &m_project, SIGNAL(songDatabaseCommandPushed()), this, SLOT(gotoSongView()) );
    connect( &m_project, SIGNAL(eventDatabaseCommandPushed()), this, SLOT(gotoEventView()) );

    connect( ui->menuVisible_attributes, SIGNAL(aboutToShow()), this, SLOT(createAttributeVisibilityMenu() ));

    connect( m_project.songDatabase(), SIGNAL(songRemoved(int)), ui->songDatabaseWidget, SLOT(updateAttachmentChooser()) );

    connect( ui->menu_Language, SIGNAL(aboutToShow()), this, SLOT( createLanguageMenu() ));


    //////////////////////////////////////////
    ///
    //////////////////////////////////////////
    connect( ui->buttonSongDatabase, &QPushButton::clicked, [this]()
    {
        selectPage( SongDatabasePage );
    });
    connect( ui->buttonEventDatabase, &QPushButton::clicked, [this]()
    {
        selectPage( EventDatabasePage );
    });
    selectPage( SongDatabasePage );

    askForSync();
}

MainWindow::~MainWindow()
{
    QSettings settings;
    settings.setValue( "Geometry", saveGeometry() );
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

Event* MainWindow::currentEvent() const
{
    return ui->eventDatabaseWidget->currentEvent();
}

void MainWindow::createAttachmentActions()
{
    // gather attachment creators
    for (const QString & classname : Creatable::classnamesInCategory("Attachment"))
    {
        QAction* action = new QAction( this );
        action->setText( QString(tr("New %1")).arg( Creatable::name(classname) ) );

        if (classname == "PDFAttachment")
        {
            action->setIcon(QIcon(":/icons/icons/pdf24.png"));
        }
        else if (classname == "ChordPatternAttachment")
        {
            action->setIcon(QIcon(":/icons/icons/write13.png"));
        }
        else if (classname == "AudioAttachment")
        {
            action->setIcon(QIcon(":/icons/icons/song1.png"));
        }
        else if (classname == "ChordPatternProxyAttachment")
        {
            action->setIcon( QIcon(":/icons/icons/link67.png") );
        }
        else
        {
            qWarning() << QString("action <create %1> has no icon.").arg( classname );
        }

        connect(action, &QAction::triggered, [this, classname]()
        {
            Song* song = currentSong();
            if (song)
            {
                SongNewAttachmentCommand* command = new SongNewAttachmentCommand( song, Creatable::create<Attachment>(classname) );
                app().pushCommand( command );
                updateActionsEnabled();
            }
        });
        m_newAttachmentActions.insert( classname, action );
    }
}

void MainWindow::setupAttachmentMenu()
{
    // gather attachment creators
    QAction* actionBefore = ui->menuAttachments->actions().first();
    for (const QString & classname : Creatable::classnamesInCategory("Attachment"))
    {
        QAction* action = newAttachment_Action( classname );
        ui->menuAttachments->insertAction(actionBefore, action );
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
            setCurrentPath( m_currentPath );   // ensure that the current filename is stored as to-default-open project
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
        QString ending = QString(".%1").arg(m_project.ending());
        if (!filename.endsWith( ending ))
        {
            filename.append(ending);
        }
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

bool MainWindow::canProjectClose()
{
    askForSync();
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
    open( m_currentPath );
}

int MainWindow::currentAttachmentIndex() const
{
    return ui->songDatabaseWidget->attachmentChooser()->currentAttachmentIndex();
}

Attachment* MainWindow::currentAttachment() const
{
    return ui->songDatabaseWidget->attachmentChooser()->currentAttachment();
}

void setEnabled( QObject* o, bool enable )
{
    QAction* a = qobject_cast<QAction*>(o);
    if (a) a->setEnabled(enable);

    QWidget* w = qobject_cast<QWidget*>(o);
    if (w) w->setEnabled(enable);
}

void MainWindow::updateActionsEnabled()
{
    bool song = !!currentSong();
    bool event = !!currentEvent();
    bool attachment = !!currentAttachment();
    bool git = app().project()->isGitRepository();

    QObjectList attachmentObjects, songObects, gitObjects, eventObjects;

    qDebug() << (song?"enable":"disable") << "actions";
    for (QAction* action : m_newAttachmentActions)
    {
        songObects << action;
    }

    gitObjects          << ui->actionSync;
    attachmentObjects   << ui->actionDuplicate_Attachment << ui->actionRename_Attachment << ui->actionDelete_Attachment;
    songObects << m_actionDelete_Song << m_actionCopy_Song << m_actionEdit_Program << m_actionEdit_Song_Tags;
    eventObjects << m_actionDelete_Event << m_actionCopy_Event << m_actionEdit_Event_Tags;

    for (QObject* o : eventObjects )        ::setEnabled( o, event       );
    for (QObject* o : songObects )          ::setEnabled( o, song        );
    for (QObject* o : attachmentObjects)    ::setEnabled( o, attachment  );
    for (QObject* o : gitObjects)           ::setEnabled( o, git         );

    bool chordPatternProxyAttachmentEnabled = false;
    if (currentAttachment() && currentAttachment()->inherits( "ChordPatternAttachment" ))
    {
        chordPatternProxyAttachmentEnabled = true;
    }
    m_newAttachmentActions["ChordPatternProxyAttachment"]->setEnabled( chordPatternProxyAttachmentEnabled );
}

MainWindow::Page MainWindow::currentPage() const
{
    switch (ui->stackedWidget->currentIndex())
    {
    case 0:
        return SongDatabasePage;
    case 1:
        return EventDatabasePage;
    default:
        assert( false );
        return (Page) -1;
    }
}

void MainWindow::gotoSongView()
{
    selectPage( SongDatabasePage );
}

void MainWindow::gotoEventView()
{
    selectPage( EventDatabasePage );
}












////////////////////////////////////////////////
////
///     Connections to Actions
//
///////////////////////////////////////////////


void MainWindow::my_on_actionNew_Song_triggered()
{
    app().pushCommand( new DatabaseNewItemCommand<Song>( m_project.songDatabase(), new Song(m_project.songDatabase()) ) );
    updateActionsEnabled();
}

void MainWindow::on_actionUndo_triggered()
{
    m_project.undo();
    updateActionsEnabled();
}

void MainWindow::on_actionRedo_triggered()
{
    m_project.redo();
    updateActionsEnabled();
}

void MainWindow::on_actionDelete_Attachment_triggered()
{
    Song* song = currentSong();
    int index = ui->songDatabaseWidget->attachmentChooser()->currentAttachmentIndex();

    if (song && index >= 0)
    {
        app().pushCommand( new SongRemoveAttachmentCommand( song, index ) );
        updateActionsEnabled();
    }
}

void MainWindow::on_actionNew_Project_triggered()
{
    newProject();
    updateActionsEnabled();
}

void MainWindow::on_actionSave_triggered()
{
    saveProject();
    updateActionsEnabled();
}

void MainWindow::on_actionSave_As_triggered()
{
    saveProjectAs();
    updateActionsEnabled();
}

void MainWindow::on_actionOpen_triggered()
{
    if (!canProjectClose())
    {
        return; // user aborted opening;
    }

    QString filename =
    QFileDialog::getOpenFileName( this,
                                  tr("Open ..."),
                                  proposedPath(),
                                  filter()              );
    if (filename.isEmpty())
    {
        return; // user aborted opening
    }


    open(filename);
}

bool MainWindow::canRemoveSong(Song *song)
{
    // check if song is used in setlist
    for (Event* event: m_project.eventDatabase()->items())
    {
        for (SetlistItem* item : event->setlist()->items())
        {
            if (item->song() == song)
            {
                return false;
            }
        }
    }

    return true;
}

void MainWindow::my_on_actionDelete_Song_triggered()
{
    Song* song = currentSong();
    if (song)
    {
        if (canRemoveSong( song ))
        {
            app().pushCommand( new DatabaseRemoveItemCommand<Song>( m_project.songDatabase(), song ) );
        }
        else
        {
            QMessageBox::warning( this,
                                  tr("Song cannot be removed"),
                                  tr("This song is currently in use and can thus not be removed."),
                                  QMessageBox::Ok,
                                  QMessageBox::NoButton );
        }
        updateActionsEnabled();
    }
}

void MainWindow::on_actionAdd_Folder_triggered()
{
    AddFilesToIndexDialog dialog( this );

    if (dialog.exec() != QDialog::Accepted)
    {
        return;
    }

    QProgressDialog pd( "Task in Progress", "Cancel", 0, -1, this );
    pd.setWindowModality( Qt::WindowModal );

    app().fileIndex().addSource( dialog.path(),
                                 dialog.includePDF(),
                                 dialog.includeMP3(),
                                 dialog.includeOgg(),
                                 dialog.includeAif() );

    QLabel* label = new QLabel(&pd);
    label->setWordWrap(true);
    pd.setLabel(label);
    pd.show();

    while (!app().fileIndex().operationIsFinished())
    {
        pd.setValue( (pd.value() + 1) % 100 );
        label->setText( QString("%1\n%2").arg(app().fileIndex().currentFilename())
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

void MainWindow::on_actionRename_Attachment_triggered()
{
    ui->songDatabaseWidget->attachmentChooser()->renameCurrentAttachment();
}

void MainWindow::on_actionDuplicate_Attachment_triggered()
{
    Song* cs = currentSong();
    assert( cs );

    int index = ui->songDatabaseWidget->attachmentChooser()->currentAttachmentIndex();
    assert( index >= 0 );

    Attachment* attachment = cs->attachments()[index];
    assert( attachment );

    app().pushCommand( new SongNewAttachmentCommand( cs, attachment->copy() ) );
    updateActionsEnabled();
}

void MainWindow::on_actionOpen_Terminal_here_triggered()
{
    QProcess::startDetached( "gnome-terminal", QStringList(), m_project.path() );
}

void MainWindow::on_actionClone_triggered()
{
    if (!canProjectClose())
    {
        return;
    }

    QUrl url = config["RecentCloneURL"].toUrl();
    CloneDialog dialog(m_identityManager, url, this);
    if (dialog.exec() == QDialog::Rejected)
    {
        return;
    }
    url = dialog.url();
    config.set("RecentCloneURL", url);

    if (!url.isValid())
    {
        return;
    }

    m_project.reset();
    setCurrentPath("");

    QProgressDialog pd( "Task in Progress", "Cancel", 0, -1, this );
    pd.setWindowModality( Qt::WindowModal );


    QLabel* label = new QLabel(&pd);
    label->setWordWrap(true);
    pd.setLabel(label);
    pd.show();

    m_project.cloneDetached( url.url() );
    while ( !m_project.detachedTaskFinished() )
    {
        pd.setValue( (pd.value() + 1) % 100 );
        label->setText( tr("Cloning.") );
        qApp->processEvents();
        QThread::msleep( 10 );
        if (pd.wasCanceled())
        {
            return;
        }
    }

    if ( m_project.detachedTaskSucceeded() )
    {
        if (m_project.needsInitialCommit())
        {
            m_project.saveToTempDir();
            m_project.initializeDetached( m_identityManager.currentIdentity() );
            while ( !m_project.detachedTaskFinished() )
            {
                pd.setValue( (pd.value() + 1) % 100 );
                label->setText( tr("Cloning.") );
                qApp->processEvents();
                QThread::msleep( 10 );
                if (pd.wasCanceled())
                {
                    return;
                }
            }

            if (!m_project.detachedTaskSucceeded())
            {
                QMessageBox::warning( this,
                                      tr("Initializing failed"),
                                      QString(tr("Failed to clone %1.").arg(url.url())),
                                      QMessageBox::Ok,
                                      QMessageBox::NoButton );
                newProject();
                updateWindowTitle();
                updateActionsEnabled();
                return;
            }
        }
    }
    else
    {
        QMessageBox::warning( this,
                              tr("Cloning failed"),
                              QString(tr("Failed to clone %1.").arg(url.url())),
                              QMessageBox::Ok,
                              QMessageBox::NoButton );
        newProject();
        updateWindowTitle();
        updateActionsEnabled();
        return;
    }

    if ( !m_project.loadFromTempDir())
    {
        QMessageBox::warning( this,
                              tr("Cannot load project"),
                              QString(tr("Failed to load cloned project.\n"
                                         "Make sure you cloned the correct repository and the repository is valid.")),
                              QMessageBox::Ok,
                              QMessageBox::NoButton );
        newProject();
    }

    updateWindowTitle();
    updateActionsEnabled();

}

void MainWindow::on_actionSync_triggered()
{

    //////////////////////////////////////////////////////////////////////////
    /// retrieve message and Identity.
    /// abort if one of them is invalid.
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

    QString password = m_identityManager.currentIdentity().password();
    QString name = m_identityManager.currentIdentity().loginName();
    if (!name.isEmpty())
    {
        if (password.isEmpty())
        {
            password = StringDialog::getPassword( QString(tr("Password for %1")).arg(name), "", tr("Password") );
            if (password.isEmpty())
            {
                return;
            }
        }
    }
    else
    {
    }


    //////////////////////////////////////////////////////////////////////////
    /// Set up progress dialog
    QProgressDialog pd( "Task in Progress", "Cancel", 0, -1, this );
    pd.setWindowModality( Qt::WindowModal );
    QLabel* label = new QLabel(&pd);
    label->setWordWrap(true);
    pd.setLabel(label);
    pd.show();
    label->setText( tr("Syncing.") );

    //////////////////////////////////////////////////////////////////////////
    /// begin syncing

    // wait until prepare is finished
    m_project.beginIndex();
    m_project.saveToTempDir();
    m_project.addAllFiles();
    m_project.commit( message, m_identityManager.currentIdentity() );
    m_project.pullOriginMasterDetached();
    while ( !m_project.detachedTaskFinished() )
    {
        qApp->processEvents();
        QThread::msleep( 10 );
        if (pd.wasCanceled())
        {
            m_project.endIndex();
            //TODO ensure that the state is not weird.
            return;
        }
    }

    if (!m_project.detachedTaskSucceeded())
    {
        QMessageBox::warning( this,
                              tr("Failed to pull from remote."),
                              tr("Cannot get changes from the repository.\n"
                                 "Check that you have access to the remote (e.g. a connection to the internet)"),
                              QMessageBox::Ok,
                              QMessageBox::NoButton );
        newProject();
        m_project.endIndex();
        return;
    }

    // resolve conflicts
    while (m_project.hasConflicts())
    {
        QList<ConflictFile*> conflictingFiles = m_project.createConflictingFiles();

        ConflictEditor editor( conflictingFiles, this );
        if (editor.exec() == QDialog::Rejected)
        {
            QMessageBox box( this );
            box.setWindowTitle( tr("Pending conflicts not allowed.") );
            box.setText( tr("There must not be conficts.") );
            box.addButton( QMessageBox::Yes );
            box.addButton( QMessageBox::No );
            box.addButton( QMessageBox::Abort );
            box.setButtonText( QMessageBox::Yes, tr("Use &local version") );
            box.setButtonText( QMessageBox::No, tr("Use &remote version") );
            switch (box.exec())
            {
            case QMessageBox::Yes:
                editor.resolveAllMine();
                break;
            case QMessageBox::No:
                editor.resolveAllTheirs();
                break;
            case QMessageBox::Abort:
            default:
                break;
            }
        }

        qDeleteAll( conflictingFiles );
        conflictingFiles.clear();
    }


    m_project.addAllFiles();
    m_project.commit( tr("Resolved Conflicts"), m_identityManager.currentIdentity() );


    m_project.mergeCommits( "Merge commits", m_identityManager.currentIdentity() );

    // wait until polish is finished
    m_project.pushOriginMasterDetached( identity );
    while ( !m_project.detachedTaskFinished() )
    {
        qApp->processEvents();
        QThread::msleep( 10 );
        if (pd.wasCanceled())
        {
            m_project.endIndex();
            //TODO ensure that the state is not weird.
            return;
        }
    }
    m_project.endIndex();

    if (!m_project.loadFromTempDir())
    {
        QMessageBox::warning( this,
                              tr("Failed to load project."),
                              tr("Cannot open merged project.\n"
                                 "Probably merging went wrong or the remote version was currupted.\n"
                                 "Fix the error by hand and commit it over the current version, then clone the fixed repository."),
                              QMessageBox::Ok,
                              QMessageBox::NoButton );
        newProject();
    }
    else if ( !m_project.detachedTaskSucceeded() )
    {
        QMessageBox::information( this,
                                  tr("Sync"),
                                  tr("Sync failed.\n"
                                     "Make sure your username/password is correct and the remote repository is reachable"),
                                  QMessageBox::Ok,
                                  QMessageBox::NoButton );
    }
    else
    {
        QMessageBox::information( this,
                                  tr("Sync"),
                                  tr("Sync succeeded"),
                                  QMessageBox::Ok,
                                  QMessageBox::NoButton );
        m_project.setIsSynchronized();
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

void MainWindow::my_on_actionNew_Event_triggered()
{
    app().pushCommand( new DatabaseNewItemCommand<Event>( m_project.eventDatabase(), new Event(m_project.eventDatabase())) );
}

void MainWindow::my_on_actionDelete_Event_triggered()
{
    Event* event = currentEvent();
    if (event)
    {
        app().pushCommand( new DatabaseRemoveItemCommand<Event>( m_project.eventDatabase(), event ));
        updateActionsEnabled();
    }
}


QAction* MainWindow::undoAction() const
{
    return ui->actionUndo;
}

QAction* MainWindow::redoAction() const
{
    return ui->actionRedo;
}

QAction* MainWindow::newAttachment_Action( const QString& classname )
{
    if (m_newAttachmentActions.contains(classname))
    {
        return m_newAttachmentActions[classname];
    }
    else
    {
        qWarning() << "Attachment " << classname << "not known." << m_newAttachmentActions.keys();
        return NULL;
    }
}

void MainWindow::my_on_actionCopy_Song_triggered()
{
    QModelIndexList selectedSongs = ui->songDatabaseWidget->songTableView()->selectionModel()->selectedIndexes();
    app().clipboard()->setMimeData( m_project.songDatabase()->mimeData( selectedSongs ) );
}

void MainWindow::my_on_actionPaste_Song_triggered()
{
    app().project()->songDatabase()->dropMimeData(app().clipboard()->mimeData(), Qt::CopyAction, m_project.songDatabase()->rowCount(), 0, QModelIndex());
}

void MainWindow::my_on_actionCopy_Event_triggered()
{
    QModelIndexList selectedSongs = ui->eventDatabaseWidget->eventTableView()->selectionModel()->selectedIndexes();
    app().clipboard()->setMimeData( m_project.eventDatabase()->mimeData( selectedSongs ) );
}

void MainWindow::my_on_actionPaste_Event_triggered()
{
    app().project()->eventDatabase()->dropMimeData(app().clipboard()->mimeData(), Qt::CopyAction, m_project.eventDatabase()->rowCount(), 0, QModelIndex());
}

void MainWindow::my_on_actionEdit_Program_triggered()
{
    if (!currentSong())
    {
        return;
    }

    ProgramDialog pd(this);
    pd.setProgram( currentSong()->program() );
    if (pd.exec() == QDialog::Accepted)
    {
        app().pushCommand( new SongEditProgramCommand( currentSong(), pd.program() ) );
    }
}

void MainWindow::createAttributeVisibilityMenu()
{
    QMenu* menu = ui->menuVisible_attributes;

    for (int i = 0; i < m_project.songDatabase()->columnCount(); ++i )
    {
        QString name = m_project.songDatabase()->headerData(i, Qt::Horizontal, Qt::DisplayRole).toString();
        QAction* action = menu->addAction( name );
        action->setCheckable( true );
        action->setChecked( m_project.songDatabase()->columnIsVisible(i) );

        connect( action, &QAction::triggered, [this, i](bool visible)
        {
            app().pushCommand( new SongDatabaseSetColumnVisibilityCommand( m_project.songDatabase(), i, visible ) );
        });

        connect( menu, SIGNAL(aboutToHide()), action, SLOT(deleteLater()) );
    }

}


void MainWindow::my_on_actionEdit_Song_Tags_triggered()
{
    SongTableView* songTableView = ui->songDatabaseWidget->songTableView();
    QModelIndexList list = songTableView->selectionModel()->selectedIndexes();
    if (!list.isEmpty())
    {
        Taggable* t = songTableView->model()->resolveItemAtIndex(list.first());
        if (t != 0)
        {
            TagDialog d(t->tags(), this);
            if (d.exec() == QDialog::Accepted)
            {
                app().pushCommand( new EditTagsCommand( t, d.tags() ) );
            }
        }
    }
}

void MainWindow::my_on_actionEdit_Event_Tags_triggered()
{
    EventTableView* eventTableView = ui->eventDatabaseWidget->eventTableView();
    QModelIndexList list = eventTableView->selectionModel()->selectedIndexes();
    if (!list.isEmpty())
    {
        Taggable* t = eventTableView->model()->resolveItemAtIndex(list.first());
        if (t != 0)
        {
            TagDialog d(t->tags(), this);
            if (d.exec() == QDialog::Accepted)
            {
                app().pushCommand( new EditTagsCommand( t, d.tags() ) );
            }
        }
    }
}

void MainWindow::open(const QString &filename)
{
    if (!filename.isEmpty())
    {
        if (QFileInfo(filename).isReadable())
        {
            if ( m_project.loadZip( filename ) )
            {
                setCurrentPath( filename );
                updateWindowTitle();
                updateActionsEnabled();
            }
            else
            {
                QMessageBox::warning( this,
                                      QString(tr("Opening %1")).arg(filename),
                                      QString(tr("Cannot open %1. Unknown file format.")).arg(filename),
                                      QMessageBox::Ok
                                      );
                setCurrentPath("");
                newProject();
            }
        }
        else
        {
            QMessageBox::warning( this,
                                  QString(tr("Opening %1")).arg(filename),
                                  QString(tr("File %1 not found.")).arg(filename),
                                  QMessageBox::Ok
                                  );
            m_project.reset();
            setCurrentPath("");
            newProject();
        }

        updateWindowTitle();
        updateActionsEnabled();
    }
}

void MainWindow::createLanguageMenu()
{
    for (const QFileInfo& fileInfo : QDir(":/translations/").entryInfoList(QStringList() << "can2*.qm", QDir::Files, QDir::Name ))
    {
        QString locale = fileInfo.baseName().mid(5); // skip ending, skip can2_
        QAction* action = new QAction( ui->menu_Language );
        action->setText( QLocale(locale).nativeLanguageName() );
        ui->menu_Language->addAction(action);
        action->setCheckable( true );
        connect(ui->menu_Language, SIGNAL(aboutToHide()), action, SLOT(deleteLater()));
        if (config["locale"] == locale)
        {
            action->setChecked(true);
        }

        connect( action, &QAction::triggered, [this, locale]()
        {
            QMessageBox::information( this,
                                      tr("Information"),
                                      tr("Language changes will apply on next start."),
                                      QMessageBox::Ok,
                                      QMessageBox::NoButton );
            config.set("locale", locale);
        });
    }
}


SongTableView* MainWindow::songTableView()
{
    return ui->songDatabaseWidget->songTableView();
}


void MainWindow::on_action_Export_all_songs_triggered()
{
    OrphantSetlist setlist( tr("All songs"));

    for( Song* song : m_project.songDatabase()->items() )
    {
        setlist.insertItem( new SetlistItem( song ) );
    }

    PDFCreator::exportSetlist( &setlist, this );
}

void MainWindow::selectPage(Page page)
{
    switch (page)
    {
    case SongDatabasePage:
        ui->stackedWidget->setCurrentIndex( 0 );
        break;
    case EventDatabasePage:
        ui->stackedWidget->setCurrentIndex( 1 );
        break;
    }
}

void MainWindow::askForSync()
{
    if (!config["AskForSynchronizing"].toBool())
    {
        return;
    }
    if (!m_project.isGitRepository())
    {
        return;
    }

    if (m_project.isSynchronized())
    {
        return;
    }

    if ( QMessageBox::Yes == QMessageBox::question(    this,
                                                       tr("Synchronize?"),
                                                       tr("The project may not be synchronized with the remote.\n"
                                                          "Do you want to synchronize them now?\n"
                                                          "You can always do this later, but it is recommended to do it now."),
                                                       QMessageBox::Yes | QMessageBox::No,
                                                       QMessageBox::Yes
                                                    )
         )
    {
        on_actionSync_triggered();
    }
}

void MainWindow::createDebugMenu()
{
#ifdef QT_DEBUG
    QMenu* menu = new QMenu( this );
    QAction* action = new QAction( menu );
    menu->addAction( action );
    menu->setTitle( "Debug" );
    action->setText("open terminal");
    ui->menuBar->addMenu( menu );
    connect( action, &QAction::triggered, [this]()
    {
        QProcess::startDetached( "gnome-terminal", QStringList(), m_project.path() );
    });
#endif
}


























