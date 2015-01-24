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
    void testSlot1();
    void testSlot2();
    void testSlot3();
    void testSlot4();
    void testSlot5();

private:
    Ui::MainWindow *ui;

    Project m_project;
};

#endif // MAINWINDOW_H
