#include "addfilestoindexdialog.h"
#include "ui_addfilestoindexdialog.h"
#include <QFileDialog>

DEFN_CONFIG( AddFilesToIndexDialog, "AddFilesToIndexDialog" );

CONFIGURABLE_ADD_ITEM_HIDDEN( AddFilesToIndexDialog, defaultpath, QDir::homePath() );
CONFIGURABLE_ADD_ITEM_HIDDEN( AddFilesToIndexDialog, includePDF, true );
CONFIGURABLE_ADD_ITEM_HIDDEN( AddFilesToIndexDialog, includeMP3, true );
CONFIGURABLE_ADD_ITEM_HIDDEN( AddFilesToIndexDialog, includeOgg, true );

AddFilesToIndexDialog::AddFilesToIndexDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddFilesToIndexDialog)
{
    ui->setupUi(this);

    ui->lineEdit->setText( config["defaultpath"].toString() );
    ui->checkBoxPDF->setChecked( config["includePDF"].toBool() );
    ui->checkBoxMP3->setChecked( config["includeMP3"].toBool() );
    ui->checkBoxOgg->setChecked( config["includeOgg"].toBool() );
}

AddFilesToIndexDialog::~AddFilesToIndexDialog()
{
    config.set( "includePDF", ui->checkBoxPDF->isChecked() );
    config.set( "includeMP3", ui->checkBoxMP3->isChecked() );
    config.set( "includeOgg", ui->checkBoxOgg->isChecked() );
    delete ui;
}

void AddFilesToIndexDialog::on_pushButton_clicked()
{
    QFileDialog::Options options = 0;
    QString filename = QFileDialog::getExistingDirectory( this,
                                                          tr("Directory ..."),
                                                          ui->lineEdit->text(),
                                                          options
                                                          );

    if (!filename.isEmpty())
    {
        ui->lineEdit->setText( filename );
    }
}

bool AddFilesToIndexDialog::includePDF() const
{
    return ui->checkBoxPDF->isChecked();
}
bool AddFilesToIndexDialog::includeMP3() const
{
    return ui->checkBoxMP3->isChecked();
}
bool AddFilesToIndexDialog::includeOgg() const
{
    return ui->checkBoxOgg->isChecked();
}

QString AddFilesToIndexDialog::path() const
{
    return ui->lineEdit->text();
}
