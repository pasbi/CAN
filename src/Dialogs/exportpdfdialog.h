#ifndef EXPORTPDFDIALOG_H
#define EXPORTPDFDIALOG_H

#include <QDialog>
#include "taggable.h"
#include "configurable.h"
#include "Database/SongDatabase/song.h"


namespace Ui {
class ExportPDFDialog;
}

class ExportPDFDialog : public QDialog
{
    Q_OBJECT
    DECL_CONFIG( ExportPDFDialog )

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
