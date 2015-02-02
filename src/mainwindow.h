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

private:
    Ui::MainWindow *ui;

    Project m_project;
    void setupAttachmentMenu();

    void setCurrentAttachment( int index );
    Song* currentSong() const;


};

#endif // MAINWINDOW_H
