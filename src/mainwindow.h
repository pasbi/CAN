#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "Project/project.h"
#include "Program/program.h"


namespace Ui {
class MainWindow;
}

template<typename T> class DatabaseView;
class Song;
class Event;
class Attachment;
class MainWindow : public QMainWindow
{
    enum Page { SongDatabasePage, EventDatabasePage };
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    QAction* undoAction() const;
    QAction* redoAction() const;
    /**
     * @brief newAttachmentAction returns the action that is used to create Attachment with given classname.
     * @param classname
     * @return QAction to create the Attachment or nullptr if no action is known to create it.
     */
    QAction* newAttachment_Action(const QString &classname);

    DatabaseView<Song> *songTableView();

public slots:
    void updateActionsEnabled();

private slots:
    void resizeSplitter();  // left column should be as small as possible.
    void on_actionUndo_triggered();
    void on_actionRedo_triggered();
    void on_actionDelete_Attachment_triggered();

    void on_actionNew_Project_triggered();

private:
    Ui::MainWindow *ui;

    Project m_project;

    // saving and loading related stuff
    bool canProjectClose();
    QString projectName() const;
    void setCurrentPath( const QString & path );
    QString filter() const { return tr("All files (*);;CAN files (*.can)"); }
    QString proposedPath() const;
    QString m_currentPath;
    bool saveProject();
    bool saveProjectAs();
    bool newProject();
    void loadDefaultProject();
    void createDebugMenu();


protected:
    void closeEvent(QCloseEvent *e);

public slots:
    void gotoSongView();
    void gotoEventView();

private slots:
    void selectPage( Page page );
    void updateWindowTitle();
    void on_actionSave_triggered();
    void on_actionSave_As_triggered();
    void on_actionOpen_triggered();
    void on_actionAdd_Folder_triggered();
    void on_actionClear_Index_triggered();
    void on_actionRename_Attachment_triggered();
    void on_actionDuplicate_Attachment_triggered();
    void on_actionSettings_triggered();
    void on_action_Index_Info_triggered();
    void on_action_Export_all_songs_triggered();
    void on_actionCopy_Indexed_Attachments_triggered();


    // my auto-connection slots
    void my_on_actionNew_Song_triggered();
    void my_on_actionDelete_Song_triggered();
    void my_on_actionCopy_Song_triggered();
    void my_on_actionPaste_Song_triggered();
    void my_on_actionEdit_Program_triggered();
    void my_on_actionEdit_Song_Tags_triggered();

    void my_on_actionNew_Event_triggered();
    void my_on_actionDelete_Event_triggered();
    void my_on_actionCopy_Event_triggered();
    void my_on_actionPaste_Event_triggered();
    void my_on_actionEdit_Event_Tags_triggered();

    void my_on_actionNew_SetlistItem_triggered();
    void my_on_actionDelete_SetlistItem_triggered();
    void my_on_actionCopy_SetlistItem_triggered();
    void my_on_actionPaste_SetlistItem_triggered();


private:
    void createLanguageMenu();
    void createAttachmentActions();
    void setupAttachmentMenu();
    QMap<QString, QAction*> m_newAttachmentActions;
    void setCurrentAttachment( int index );
    Song* currentSong() const;
    Event* currentEvent() const;

    int currentAttachmentIndex() const;
    Attachment* currentAttachment() const;
    Page currentPage() const;
    bool canRemoveSong( Song* song );

    void open(const QString& filename);

    QAction* m_actionNew_Song;
    QAction* m_actionDelete_Song;
    QAction* m_actionCopy_Song;
    QAction* m_actionPaste_Song;
#ifdef HAVE_PROGRAM
    QAction* m_actionEdit_Program;
#endif
    QAction* m_actionEdit_Song_Tags;

    QAction* m_actionNew_Event;
    QAction* m_actionDelete_Event;
    QAction* m_actionCopy_Event;
    QAction* m_actionPaste_Event;
    QAction* m_actionEdit_Event_Tags;

    QAction* m_actionNew_SetlistItem;
    QAction* m_actionDelete_SetlistItem;
    QAction* m_actionCopy_SetlistItem;
    QAction* m_actionPaste_SetlistItem;

    friend class Application;   // application handles some quasi-global functions
};

#endif // MAINWINDOW_H
