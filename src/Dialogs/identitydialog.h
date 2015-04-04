#ifndef IDENTITYDIALOG_H
#define IDENTITYDIALOG_H

#include <QDialog>
#include "identitymanager.h"

namespace Ui {
class IdentityDialog;
}

class IdentityDialog : public QDialog
{
    Q_OBJECT

public:
    explicit IdentityDialog(IdentityManager *manager, QWidget *parent = 0);
    ~IdentityDialog();

public slots:
    void accept();
    void reject();

private slots:
    void setEditsAndIcons();
    void on_buttonAdd_clicked();
    void on_buttonRemove_clicked();

    void on_nameEdit_textChanged(const QString &);
    void on_emailEdit_textChanged(const QString &);
    void on_loginEdit_textChanged(const QString &);
    void on_passwordEdit_textChanged(const QString &);

private:
    Ui::IdentityDialog *ui;
    IdentityManager* m_manager;

    QList<Identity> m_identities;   // hold a copy to be able to do cancel

    void setIdentity();
    void setEdits();
    void setComboBox();
    void setIcon();

    int i() const;
    Identity* currentIdentity();

};

#endif // IDENTITYDIALOG_H
