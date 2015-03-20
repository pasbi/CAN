#ifndef PROGRESSDIALOG_H
#define PROGRESSDIALOG_H

#include <QDialog>
#include <QTime>
#include <functional>

namespace Ui {
class ProgressDialog;
}

class ProgressDialog : public QDialog
{
    Q_OBJECT
public:
    explicit ProgressDialog(QWidget *parent = 0);
    ~ProgressDialog();
    void setUpdateCallback(std::function< QString(void) > function, int msec = 200 );
    void setText( const QString & text );

private:
    Ui::ProgressDialog *ui;
    bool m_functionSet = false;
    std::function< QString(void) > m_function;
    QTimer* m_timer;

};

#endif // PROGRESSDIALOG_H
