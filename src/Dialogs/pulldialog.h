#ifndef PULLDIALOG_H
#define PULLDIALOG_H

#include <QDialog>

namespace Ui {
class PullDialog;
}

class PullDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PullDialog(QWidget *parent = 0);
    ~PullDialog();

private:
    Ui::PullDialog *ui;
};

#endif // PULLDIALOG_H
