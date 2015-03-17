#include "identitydialog.h"
#include "ui_identitydialog.h"
#include <QMessageBox>

IdentityDialog::IdentityDialog(IdentityManager* manager, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::IdentityDialog),
    m_manager( manager )
{
    ui->setupUi(this);
    updateList();
    ui->comboBox->setCurrentIndex( manager->currentIdentityIndex() );
    if (m_manager->size() > 0)
    {
        on_comboBox_currentIndexChanged( manager->currentIdentityIndex() );
    }


    ui->buttonUndo->setEnabled( manager->undoStack()->canUndo() );
    ui->buttonRedo->setEnabled( manager->undoStack()->canRedo() );
    connect( manager->undoStack(), SIGNAL(canRedoChanged(bool)), ui->buttonRedo, SLOT(setEnabled(bool)));
    connect( manager->undoStack(), SIGNAL(canUndoChanged(bool)), ui->buttonUndo, SLOT(setEnabled(bool)));
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

void IdentityDialog::updateIcons()
{
    for (int i = 0; i < ui->comboBox->count(); ++i)
    {
        ui->comboBox->setItemIcon( i, QIcon() );
    }
    ui->comboBox->setItemIcon( ui->comboBox->currentIndex(), QIcon(":/icons/icons/check40.png") );
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

    m_manager->setCurrentIndex( index );
    updateIcons();

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
    updateIcons();
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

    ui->emailEdit->setText( m_manager->currentIdentity().email() );
    ui->nameEdit->setText( m_manager->currentIdentity().name() );

    unblock();
    updateIcons();
}

void IdentityDialog::updateIdentity()
{
    if (i() >= 0 && i() < m_manager->size())
    {
        m_manager->edit( i(), ui->nameEdit->text(), ui->emailEdit->text() );
        updateList();
    }
}

void IdentityDialog::accept()
{
    m_manager->removeInvalidIdentities();
    if (m_manager->size() == 0)
    {
        QMessageBox messageBox( this );
        messageBox.setWindowTitle( tr("No Identities") );
        messageBox.setText( tr("You should provide at least one identity. "
                               "Without identities, you cannot sync.\n"
                               "Identities with empty name or email are not allowed.") );
        messageBox.addButton( QMessageBox::Ok );
        messageBox.addButton( QMessageBox::Cancel );
        messageBox.setButtonText( QMessageBox::Ok, tr("Edit ...") );
        messageBox.setButtonText( QMessageBox::Cancel, tr("Proceed") );
        messageBox.setDefaultButton( QMessageBox::Ok );
        if (messageBox.exec() == QMessageBox::Ok)
        {
            return;
        }
    }
    QDialog::accept();
}


void IdentityDialog::reject()
{
    accept();
}

void IdentityDialog::on_buttonUndo_clicked()
{
    m_manager->undoStack()->undo();
    updateList();
}

void IdentityDialog::on_buttonRedo_clicked()
{
    m_manager->undoStack()->redo();
    updateList();
}

