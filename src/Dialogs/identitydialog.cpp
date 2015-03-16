#include "identitydialog.h"
#include "ui_identitydialog.h"

IdentityDialog::IdentityDialog(IdentityManager* manager, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::IdentityDialog),
    m_manager( manager )
{
    ui->setupUi(this);
    ui->comboBox->setCurrentIndex( 0 );
    if (m_manager->size() > 0)
    {
        on_comboBox_currentIndexChanged(0);
    }
    updateList();
}

IdentityDialog::~IdentityDialog()
{
    delete ui;
}

void IdentityDialog::on_nameEdit_textChanged(const QString &)
{
    updateIdentity();
}

void IdentityDialog::on_emailEdit_textChanged(const QString &)
{
    updateIdentity();
}

void IdentityDialog::on_nameEdit_editingFinished()
{
    updateIdentity();
}

void IdentityDialog::on_emailEdit_editingFinished()
{
    updateIdentity();
}

void IdentityDialog::block(bool bl)
{
    ui->emailEdit->blockSignals(bl);
    ui->nameEdit->blockSignals(bl);
    ui->comboBox->blockSignals(bl);
}

void IdentityDialog::on_comboBox_currentIndexChanged(int index)
{
    block();

    ui->nameEdit->setText(  index >= 0 ?
                                m_manager->identity(index).name()
                              : "" );
    ui->emailEdit->setText( index >= 0 ?
                                m_manager->identity(index).email()
                              : "" );

    unblock();
}

void IdentityDialog::on_buttonAdd_clicked()
{
    m_manager->addIdentity(Identity());
    updateList();


    block();
    ui->nameEdit->clear();
    ui->emailEdit->clear();
    unblock();
    ui->comboBox->setCurrentIndex( m_manager->size() - 1 );
}

void IdentityDialog::on_buttonRemove_clicked()
{
    m_manager->removeIdentity( i() );
    updateList();
}

int IdentityDialog::i() const
{
    return ui->comboBox->currentIndex();
}

void IdentityDialog::updateList()
{
    block();

    int index = i();
    ui->comboBox->clear();
    ui->comboBox->addItems( m_manager->identities() );
    index = qMax( index, 0 );
    index = qMin( index, ui->comboBox->count() - 1 );
    ui->comboBox->setCurrentIndex( index );

    ui->buttonRemove->setEnabled( m_manager->size() > 0 );
    ui->emailEdit->setEnabled( m_manager->size() > 0 );
    ui->nameEdit->setEnabled( m_manager->size() > 0 );
    if (m_manager->size() == 0)
    {
        ui->emailEdit->clear();
        ui->nameEdit->clear();
    }

    unblock();
}

void IdentityDialog::updateIdentity()
{
    if (i() >= 0 && i() < m_manager->size())
    {
        m_manager->edit( i(), ui->nameEdit->text(), ui->emailEdit->text() );
        updateList();
    }
}

