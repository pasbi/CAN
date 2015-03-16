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
    void on_comboBox_currentIndexChanged(int index);
    void on_buttonAdd_clicked();
    void on_buttonRemove_clicked();
    int i() const;
    void updateList();
    void on_nameEdit_textChanged(const QString &);
    void on_emailEdit_textChanged(const QString &);

    void on_nameEdit_editingFinished();

    void on_emailEdit_editingFinished();


private:
    Ui::IdentityDialog *ui;
    IdentityManager* m_manager;
    void updateIdentity();
    void block(bool bl);
    void block() { block( true ); }
    void unblock() { block( false); }

};

#endif // IDENTITYDIALOG_H
