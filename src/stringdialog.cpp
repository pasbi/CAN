#include "stringdialog.h"
#include "ui_stringdialog.h"

StringDialog::StringDialog(const QString & text, const QString & placeHolderText = "", QWidget *parent) :
    QDialog(parent),
    ui(new Ui::StringDialog)
{
    ui->setupUi(this);
    ui->lineEdit->setPlaceholderText( placeHolderText );
    ui->lineEdit->setText( text );
}

StringDialog::~StringDialog()
{
    delete ui;
}

QString StringDialog::string() const
{
    return ui->lineEdit->text();
}

QString StringDialog::getString( const QString &text, const QString &placeHolderText, QWidget* parent)
{
    StringDialog dialog(text, placeHolderText, parent);
    int r = dialog.exec();
    if (r == QDialog::Accepted)
    {
        return dialog.string();
    }
    else
    {
        return "";
    }
}
