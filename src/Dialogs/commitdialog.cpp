#include "commitdialog.h"
#include "ui_commitdialog.h"
#include "identitydialog.h"

CommitDialog::CommitDialog(IdentityManager* identityManager, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CommitDialog),
    m_identityManager( identityManager )
{
    ui->setupUi(this);
    ui->comboBox->addItems( m_identityManager->identities() );
    connect(ui->comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(updateButtonEnabled()));
    connect(ui->textEdit, SIGNAL(textChanged()), this, SLOT(updateButtonEnabled()));
    updateButtonEnabled();
}

CommitDialog::~CommitDialog()
{
    delete ui;
}

QString CommitDialog::message() const
{
    return ui->textEdit->toPlainText();
}

Identity CommitDialog::identity() const
{
    int i = ui->comboBox->currentIndex();
    return m_identityManager->identity( i );
}

void CommitDialog::on_buttonEditIdentities_clicked()
{
    IdentityDialog dialog(m_identityManager, this);
    dialog.exec();

    ui->comboBox->clear();
    ui->comboBox->addItems(m_identityManager->identities());
}

void CommitDialog::updateButtonEnabled()
{
    int index = ui->comboBox->currentIndex();
    bool enabled  = index >= 0;
         enabled &= index < m_identityManager->size();
    if (enabled)
    {
        enabled &= m_identityManager->identity(index).isValid();
    }
    enabled &= !ui->textEdit->toPlainText().isEmpty();

    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled( enabled );
}
