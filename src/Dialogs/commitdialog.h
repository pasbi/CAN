#ifndef COMMITDIALOG_H
#define COMMITDIALOG_H

#include <QDialog>
#include "../../../ZipGit/src/zipgit.h"

namespace Ui {
class CommitDialog;
}

class CommitDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CommitDialog(QWidget *parent = 0);
    ~CommitDialog();


    QString message() { return "empty message"; }
    Identity identity() { return Identity("detlef", "emaiL"); }

private:
    Ui::CommitDialog *ui;
};

#endif // COMMITDIALOG_H
