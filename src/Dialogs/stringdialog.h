#ifndef STRINGDIALOG_H
#define STRINGDIALOG_H

#include <QDialog>

namespace Ui {
class StringDialog;
}

class StringDialog : public QDialog
{
    Q_OBJECT

public:
    explicit StringDialog(const QString& title, const QString &text = "", const QString &placeHolderText = "", QWidget *parent = 0);
    ~StringDialog();

    static QString getString(const QString &title, const QString &text, const QString &placeHolderText);
    static QString getPassword(const QString& title, const QString &defaultpassword, const QString &placeHolderText);
    static QString getURL(const QString& title, const QString & url, const QString & placeHolderText);
    QString string() const;

private:
    Ui::StringDialog *ui;

private slots:
    void spawnFileDialog();

protected:
    bool eventFilter(QObject *o, QEvent *e);
};

#endif // STRINGDIALOG_H
