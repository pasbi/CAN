#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "project.h"


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void resizeSplitter();  // left column should be as small as possible.
    void on_actionNew_Song_triggered();
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
    bool openProject();
    bool newProject();

private slots:
    void updateWindowTitle();
    void on_actionSave_triggered();

    void on_actionSave_As_triggered();

    void on_actionOpen_triggered();





    void setupAttachmentMenu();
    void setCurrentAttachment( int index );
    Song* currentSong() const;
};

#endif // MAINWINDOW_H
