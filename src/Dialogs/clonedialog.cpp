#include "clonedialog.h"
#include "ui_clonedialog.h"
#include <QFileDialog>
#include "identitydialog.h"

CloneDialog::CloneDialog(IdentityManager &identityManager, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CloneDialog),
    m_identityManager( &identityManager )
{
    ui->setupUi(this);
    ui->comboBox->addItems( m_identityManager->identities() );
    ui->comboBox->setCurrentIndex( m_identityManager->currentIdentityIndex() );
    connect(ui->comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(updateButtonEnabled()));
    connect(ui->lineEdit, SIGNAL(textChanged(QString)), this, SLOT(updateButtonEnabled()));
    updateButtonEnabled();
    ui->lineEdit->setText( QDir::homePath() );
}

CloneDialog::~CloneDialog()
{
    delete ui;
}

Identity CloneDialog::identity() const
{
    int i = ui->comboBox->currentIndex();
    return m_identityManager->identity( i );
}

void CloneDialog::on_buttonEditIdentities_clicked()
{
    IdentityDialog dialog(m_identityManager, this);
    dialog.exec();

    ui->comboBox->clear();
    ui->comboBox->addItems(m_identityManager->identities());
    ui->comboBox->setCurrentIndex( m_identityManager->currentIdentityIndex() );
}

void CloneDialog::updateButtonEnabled()
{
    int index = ui->comboBox->currentIndex();
    bool enabled  = index >= 0;
         enabled &= index < m_identityManager->size();
    if (enabled)
    {
        enabled &= m_identityManager->identity(index).isValid();
    }
    enabled &= !ui->lineEdit->text().isEmpty();

    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled( enabled );
}

void CloneDialog::on_pushButtonOpenFileDialog_clicked()
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

QUrl CloneDialog::url() const
{
    return QUrl::fromUserInput( ui->lineEdit->text() );
}
