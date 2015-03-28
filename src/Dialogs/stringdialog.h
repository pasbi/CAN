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
    explicit StringDialog(const QString &text, const QString &placeHolderText, QWidget *parent = 0);
    ~StringDialog();

    static QString getString( const QString &text, const QString &placeHolderText, QWidget* parent);
    QString string() const;

private:
    Ui::StringDialog *ui;
};

#endif // STRINGDIALOG_H
