#include "stringdialog.h"
#include "ui_stringdialog.h"
#include <QFileDialog>

StringDialog::StringDialog(const QString& title, const QString & text, const QString & placeHolderText, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::StringDialog)
{
    ui->setupUi(this);
    ui->lineEdit->setPlaceholderText( placeHolderText );
    ui->lineEdit->setText( text );

    setWindowTitle( title );

    connect( ui->buttonFileDialog, SIGNAL(pressed()), this, SLOT(spawnFileDialog()) );
}

StringDialog::~StringDialog()
{
    delete ui;
}

QString StringDialog::string() const
{
    return ui->lineEdit->text();
}

QString StringDialog::getString( const QString& title, const QString &text, const QString &placeHolderText)
{
    StringDialog dialog(title, text, placeHolderText);
    dialog.ui->buttonFileDialog->hide();
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

QString StringDialog::getPassword( const QString& title, const QString &password, const QString &placeHolderText)
{
    StringDialog dialog(title, password, placeHolderText);
    dialog.ui->buttonFileDialog->hide();
    dialog.ui->lineEdit->setEchoMode( QLineEdit::Password );
    connect( dialog.ui->lineEdit, &QLineEdit::textChanged, [&dialog](QString text)
    {
        dialog.ui->buttonOk->setEnabled( !text.isEmpty() );
    });
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


QString StringDialog::getURL( const QString& title, const QString &url, const QString &placeHolderText)
{
    StringDialog dialog(title, url, placeHolderText);
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

void StringDialog::spawnFileDialog()
{
    QFileDialog dialog;
    dialog.setDirectory( ui->lineEdit->text() );
    dialog.setFileMode( QFileDialog::Directory );
    if ( QDialog::Accepted == dialog.exec() )
    {
        if (!dialog.selectedUrls().isEmpty())
        {
            ui->lineEdit->setText( dialog.selectedUrls().first().url() );
        }
        accept();
    }
    else
    {
        // on reject, do nothing.
    }

}








