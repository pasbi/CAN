#ifndef FILEINDEXDIALOG_H
#define FILEINDEXDIALOG_H

#include <QDialog>

namespace Ui {
class FileIndexDialog;
}

class FileIndexDialog : public QDialog
{
    Q_OBJECT

public:
    explicit FileIndexDialog(QWidget *parent = 0);
    ~FileIndexDialog();

private:
    Ui::FileIndexDialog *ui;
};

#endif // FILEINDEXDIALOG_H
