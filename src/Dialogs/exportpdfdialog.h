#ifndef EXPORTPDFDIALOG_H
#define EXPORTPDFDIALOG_H

#include <QDialog>
#include "taggable.h"


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

    bool test( const Attachment *taggable ) const;
    bool test( const Song* song ) const;

public slots:
    void accept();

private slots:
    void on_buttonEditPreferences_clicked();

private:
    Ui::ExportPDFDialog *ui;
};

#endif // EXPORTPDFDIALOG_H
