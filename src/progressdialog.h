#ifndef PROGRESSDIALOG_H
#define PROGRESSDIALOG_H

#include <QDialog>
#include <QTime>

namespace Ui {
class ProgressDialog;
}

class ProgressDialog : public QDialog
{
    Q_OBJECT

public:
    ~ProgressDialog();

    static void show();
    static void hide();
    static bool isCanceled();
    static void processEvents();


private:
    explicit ProgressDialog(QWidget *parent = 0);
    Ui::ProgressDialog *ui;
    static ProgressDialog* m_singleton;
    static ProgressDialog* singleton();
    static QTime m_timeOfLastEventLoopProcessing;
};

#endif // PROGRESSDIALOG_H
