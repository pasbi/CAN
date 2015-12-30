#include "exportpdfdialog.h"
#include "ui_exportpdfdialog.h"
#include "preferencedialog.h"
#include "Attachments/attachment.h"
#include "Database/SongDatabase/song.h"
#include "application.h"


ExportPDFDialog::ExportPDFDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ExportPDFDialog)
{
    ui->setupUi(this);
}

ExportPDFDialog::~ExportPDFDialog()
{
    delete ui;
}
