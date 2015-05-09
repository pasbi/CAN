#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "project.h"
#include "configurable.h"
#include "identitymanager.h"


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    DECL_CONFIG( MainWindow )
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    QAction* undoAction() const;
    QAction* redoAction() const;

private slots:
    void resizeSplitter();  // left column should be as small as possible.
    void on_actionUndo_triggered();
    void on_actionRedo_triggered();
    void on_actionDelete_Attachment_triggered();

    void on_actionNew_Project_triggered();

private:
    Ui::MainWindow *ui;

    Project m_project;
    IdentityManager m_identityManager;

    // saving and loading related stuff
    bool canProjectClose();
    QString projectName() const;
    void setCurrentPath( const QString & path );
    QString filter() const { return tr("All files (*);;CAN files (*.can)"); }
    QString proposedPath() const;
    QString m_currentPath;
    bool saveProject();
    bool saveProjectAs();
    bool openProject();
    bool newProject();
    void loadDefaultProject();

protected:
    void closeEvent(QCloseEvent *e);

private slots:
    void gotoSongView();
    void gotoEventView();
    void updateWindowTitle();
    void on_actionSave_triggered();
    void on_actionSave_As_triggered();
    void on_actionOpen_triggered();
    void on_actionAdd_Folder_triggered();
    void on_actionClear_Index_triggered();
    void on_actionRename_Attachment_triggered();
    void on_actionDuplicate_Attachment_triggered();
    void updateWhichWidgetsAreEnabled();
    void on_actionOpen_Terminal_here_triggered();
    void on_actionClone_triggered();
    void on_actionSync_triggered();
    void on_actionIdentites_triggered();
    void on_actionSettings_triggered();
    void on_action_Index_Info_triggered();

    void my_on_actionNew_Song_triggered();
    void my_on_actionDelete_Song_triggered();
    void my_on_actionCopy_Song_triggered();
    void my_on_actionPaste_Song_triggered();

    void my_on_actionNew_Event_triggered();
    void my_on_actionDelete_Event_triggered();
    void my_on_actionCopy_Event_triggered();
    void my_on_actionPaste_Event_triggered();


private:
    void setupAttachmentMenu();
    void setCurrentAttachment( int index );
    Song* currentSong() const;
    Event* currentEvent() const;

    int currentAttachmentIndex() const;
    Attachment* currentAttachment() const;
    enum Page { SongDatabasePage, EventDatabasePage };
    Page currentPage() const;
    bool canRemoveSong( Song* song );

};

#endif // MAINWINDOW_H
