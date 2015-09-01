#ifndef CLONEDIALOG_H
#define CLONEDIALOG_H

#include <QDialog>
#include <QUrl>

namespace Ui {
class CloneDialog;
}
class IdentityManager;
class Identity;
class CloneDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CloneDialog(IdentityManager& identityManager, const QUrl& defaultURL, QWidget *parent = 0);
    ~CloneDialog();

    QUrl url() const;
    Identity identity() const;

private slots:
    void on_buttonEditIdentities_clicked();
    void updateButtonEnabled();

    void on_pushButtonOpenFileDialog_clicked();

private:
    Ui::CloneDialog *ui;
    IdentityManager* m_identityManager;
};

#endif // CLONEDIALOG_H
