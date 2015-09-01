#ifndef COMMITDIALOG_H
#define COMMITDIALOG_H

#include <QDialog>

namespace Ui {
class CommitDialog;
}
class IdentityManager;
class Identity;
class CommitDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CommitDialog(IdentityManager *identityManager, QWidget *parent = 0);
    ~CommitDialog();


    QString message() const;
    Identity identity() const;

private slots:
    void on_buttonEditIdentities_clicked();
    void updateButtonEnabled();

private:
    Ui::CommitDialog *ui;
    IdentityManager* m_identityManager;
};

#endif // COMMITDIALOG_H
