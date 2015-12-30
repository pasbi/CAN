#ifndef EXPORTPDFDIALOG_H
#define EXPORTPDFDIALOG_H

#include <QDialog>
//TODO no advantage over QDialog


namespace Ui {
class ExportPDFDialog;
}
class Song;
class Attachment;
class ExportPDFDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ExportPDFDialog(QWidget *parent = 0);
    ~ExportPDFDialog();

private:
    Ui::ExportPDFDialog *ui;
};

#endif // EXPORTPDFDIALOG_H
