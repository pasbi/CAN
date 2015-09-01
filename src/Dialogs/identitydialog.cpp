#include "identitydialog.h"
#include "ui_identitydialog.h"
#include <QMessageBox>
#include <QSignalBlocker>

#include "Project/identity.h"
#include "identitymanager.h"


IdentityDialog::IdentityDialog(IdentityManager* manager, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::IdentityDialog),
    m_manager( manager )
{
    ui->setupUi(this);
    for (int i = 0; i < manager->size(); ++i)
    {
        m_identities << manager->identity(i);
    }

    setComboBox();

    ui->comboBox->setCurrentIndex( manager->currentIdentityIndex() );
    setEditsAndIcons();

    connect( ui->comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(setEditsAndIcons()) );
}

IdentityDialog::~IdentityDialog()
{
    delete ui;
}

void IdentityDialog::on_nameEdit_textChanged(const QString &)
{
    setIdentity();
    setComboBox();
}

void IdentityDialog::on_emailEdit_textChanged(const QString &)
{
    setIdentity();
    setComboBox();
}

void IdentityDialog::on_loginEdit_textChanged(const QString &)
{
    setIdentity();
    setComboBox();
}

void IdentityDialog::on_passwordEdit_textChanged(const QString &)
{
    setIdentity();
    setComboBox();
}

int IdentityDialog::i() const
{
    return ui->comboBox->currentIndex();
}

Identity* IdentityDialog::currentIdentity()
{
    int index = i();
    if (index < 0)
    {
        return NULL;
    }
    else
    {
        return &m_identities[index];
    }
}



QString identityLabel( const Identity& identity )
{
    if (identity.email().isEmpty())
    {
        return identity.name();
    }
    else
    {
        return QString("%1 [%2]").arg(identity.name()).arg(identity.email());
    }
}

void IdentityDialog::setComboBox()
{
    int index = i();
    ui->comboBox->blockSignals(true);
    ui->comboBox->clear();
    for (const Identity & identity : m_identities)
    {
        ui->comboBox->addItem( identityLabel(identity) );
    }

    if (index == -1)
    {
        index = 0;
    }


    index = qMin( index, ui->comboBox->count() - 1 );
    ui->comboBox->setCurrentIndex( index );

    setIcon();
    ui->comboBox->blockSignals(false);

}

void IdentityDialog::setIcon()
{
    for (int i = 0; i < ui->comboBox->count(); ++i)
    {
        ui->comboBox->setItemIcon( i, QIcon() );
    }
    ui->comboBox->setItemIcon( ui->comboBox->currentIndex(), QIcon(":/icons/icons/check40.png") );
}

void IdentityDialog::setEditsAndIcons()
{
    ui->nameEdit->blockSignals( true );
    ui->emailEdit->blockSignals( true );
    ui->loginEdit->blockSignals( true );
    ui->passwordEdit->blockSignals( true );
    ui->buttonRemove->blockSignals( true );

    setEdits();
    setIcon();

    ui->buttonRemove->blockSignals( false );
    ui->nameEdit->blockSignals( false );
    ui->emailEdit->blockSignals( false );
    ui->loginEdit->blockSignals( false );
    ui->passwordEdit->blockSignals( false );
}

void IdentityDialog::on_buttonAdd_clicked()
{
    m_identities.append( Identity() );
    setComboBox();
    ui->comboBox->setCurrentIndex( ui->comboBox->count() - 1 );
    setEditsAndIcons();  // index-change-signal will be blocked in setComboBox();

}

void IdentityDialog::on_buttonRemove_clicked()
{
    m_identities.removeAt( i() );
    setComboBox();
    setEditsAndIcons();  // index-change-signal will be blocked in setComboBox();
}

void IdentityDialog::setIdentity()
{
    if (currentIdentity())
    {
        currentIdentity()->setName( ui->nameEdit->text() );
        currentIdentity()->setEmail( ui->emailEdit->text() );
        currentIdentity()->setLoginName( ui->loginEdit->text() );
        currentIdentity()->setPassword( ui->passwordEdit->text() );
    }
}

void IdentityDialog::setEdits()
{
    if (currentIdentity())
    {
        ui->nameEdit->setText( currentIdentity()->name() );
        ui->emailEdit->setText( currentIdentity()->email() );
        ui->loginEdit->setText( currentIdentity()->loginName() );
        ui->passwordEdit->setText( currentIdentity()->password() );
    }
    else
    {
        ui->nameEdit->setText( "" );
        ui->emailEdit->setText( "" );
        ui->loginEdit->setText( "" );
        ui->passwordEdit->setText( "" );
    }

    ui->nameEdit->setEnabled( !!currentIdentity() );
    ui->emailEdit->setEnabled( !!currentIdentity() );
    ui->loginEdit->setEnabled( !!currentIdentity() );
    ui->passwordEdit->setEnabled( !!currentIdentity() );
    ui->buttonRemove->setEnabled( !!currentIdentity() );
}

void IdentityDialog::accept()
{
    m_manager->setIdentities( m_identities );
    m_manager->setCurrentIndex( i() );
    QDialog::accept();
}


void IdentityDialog::reject()
{
    accept();
}


