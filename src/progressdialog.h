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
    explicit ProgressDialog(QWidget *parent = 0);
    ~ProgressDialog();

    void processEvents();


private:
    Ui::ProgressDialog *ui;
    QTime m_timeOfLastEventLoopProcessing;
};

#endif // PROGRESSDIALOG_H
