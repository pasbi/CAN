#include "textdialog.h"
#include "ui_textdialog.h"

TextDialog::TextDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TextDialog)
{
    ui->setupUi(this);
}

TextDialog::~TextDialog()
{
    delete ui;
}

void TextDialog::accept()
{
    m_text = ui->textEdit->toPlainText();
    QDialog::accept();
}

void TextDialog::setText(const QString &text)
{
    m_text = text;
    ui->textEdit->setText(text);
}

void TextDialog::closeEvent(QCloseEvent *e)
{
    QDialog::closeEvent(e);
}
