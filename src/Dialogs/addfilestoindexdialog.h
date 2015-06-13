#ifndef ADDFILESTOINDEXDIALOG_H
#define ADDFILESTOINDEXDIALOG_H

#include <QDialog>
#include "configurable.h"

namespace Ui {
class AddFilesToIndexDialog;
}

class AddFilesToIndexDialog : public QDialog
{
    Q_OBJECT
    DECL_CONFIG( AddFilesToIndexDialog )


public:
    explicit AddFilesToIndexDialog(QWidget *parent = 0);
    ~AddFilesToIndexDialog();

private slots:
    void on_pushButton_clicked();

public:
    QString path() const;
    bool includePDF() const;
    bool includeMP3() const;
    bool includeOgg() const;

private:
    Ui::AddFilesToIndexDialog *ui;
};

#endif // ADDFILESTOINDEXDIALOG_H
