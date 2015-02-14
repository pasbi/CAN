#ifndef PUSHDIALOG_H
#define PUSHDIALOG_H

#include <QDialog>

namespace Ui {
class PushDialog;
}

class PushDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PushDialog(QWidget *parent = 0);
    ~PushDialog();

private:
    Ui::PushDialog *ui;
};

#endif // PUSHDIALOG_H
