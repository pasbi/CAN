#include "programdialog.h"
#include "ui_programdialog.h"

const int NUM_PROGRAM_BUTTONS = 5;

ProgramDialog::ProgramDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ProgramDialog)
{
    ui->setupUi(this);
}

ProgramDialog::~ProgramDialog()
{
    delete ui;
}

void ProgramDialog::setProgram(const Program &program)
{
    m_program = program;
    if (!m_program.valid)
    {
        m_program.bank = -1;
        m_program.program = -1;
    }
    ui->buttonBankA->setChecked( m_program.bank == 0 );
    ui->buttonBankB->setChecked( m_program.bank == 1 );
    ui->buttonBankC->setChecked( m_program.bank == 2 );
    ui->buttonBankD->setChecked( m_program.bank == 3 );

    ui->buttonProgram1->setChecked( m_program.program % NUM_PROGRAM_BUTTONS == 0 );
    ui->buttonProgram2->setChecked( m_program.program % NUM_PROGRAM_BUTTONS == 1 );
    ui->buttonProgram3->setChecked( m_program.program % NUM_PROGRAM_BUTTONS == 2 );
    ui->buttonProgram4->setChecked( m_program.program % NUM_PROGRAM_BUTTONS == 3 );
    ui->buttonProgram5->setChecked( m_program.program % NUM_PROGRAM_BUTTONS == 4 );

    ui->spinBoxPage->setValue( m_program.program / NUM_PROGRAM_BUTTONS + 1 );

    ui->buttonNoProgram->setChecked( !m_program.valid );
}

void ProgramDialog::on_buttonBankA_clicked()
{
    ui->buttonBankB->setChecked( false );
    ui->buttonBankC->setChecked( false );
    ui->buttonBankD->setChecked( false );
    updateBankPageProgram();
}

void ProgramDialog::on_buttonBankB_clicked()
{
    ui->buttonBankA->setChecked( false );
    ui->buttonBankC->setChecked( false );
    ui->buttonBankD->setChecked( false );
    updateBankPageProgram();
}

void ProgramDialog::on_buttonBankC_clicked()
{
    ui->buttonBankA->setChecked( false );
    ui->buttonBankB->setChecked( false );
    ui->buttonBankD->setChecked( false );
    updateBankPageProgram();
}

void ProgramDialog::on_buttonBankD_clicked()
{
    ui->buttonBankA->setChecked( false );
    ui->buttonBankB->setChecked( false );
    ui->buttonBankC->setChecked( false );
    updateBankPageProgram();
}

void ProgramDialog::on_spinBoxPage_valueChanged(int)
{
    updateBankPageProgram();
}

void ProgramDialog::on_buttonProgram1_clicked()
{
    ui->buttonProgram2->setChecked( false );
    ui->buttonProgram3->setChecked( false );
    ui->buttonProgram4->setChecked( false );
    ui->buttonProgram5->setChecked( false );
    updateBankPageProgram();
}

void ProgramDialog::on_buttonProgram2_clicked()
{
    ui->buttonProgram1->setChecked( false );
    ui->buttonProgram3->setChecked( false );
    ui->buttonProgram4->setChecked( false );
    ui->buttonProgram5->setChecked( false );
    updateBankPageProgram();
}

void ProgramDialog::on_buttonProgram3_clicked()
{
    ui->buttonProgram1->setChecked( false );
    ui->buttonProgram2->setChecked( false );
    ui->buttonProgram4->setChecked( false );
    ui->buttonProgram5->setChecked( false );
    updateBankPageProgram();
}

void ProgramDialog::on_buttonProgram4_clicked()
{
    ui->buttonProgram1->setChecked( false );
    ui->buttonProgram2->setChecked( false );
    ui->buttonProgram3->setChecked( false );
    ui->buttonProgram5->setChecked( false );
    updateBankPageProgram();
}

void ProgramDialog::on_buttonProgram5_clicked()
{
    ui->buttonProgram1->setChecked( false );
    ui->buttonProgram2->setChecked( false );
    ui->buttonProgram3->setChecked( false );
    ui->buttonProgram4->setChecked( false );
    updateBankPageProgram();
}

void ProgramDialog::on_buttonNoProgram_clicked()
{
    ui->buttonProgram1->setChecked( false );
    ui->buttonProgram2->setChecked( false );
    ui->buttonProgram3->setChecked( false );
    ui->buttonProgram4->setChecked( false );
    ui->buttonProgram5->setChecked( false );
    ui->buttonBankA->setChecked( false );
    ui->buttonBankB->setChecked( false );
    ui->buttonBankC->setChecked( false );
    ui->buttonBankD->setChecked( false );
    ui->spinBoxDirectProgram->blockSignals(true);
    ui->spinBoxPage->setValue( 0 );
    ui->spinBoxDirectProgram->blockSignals(false);
    updateBankPageProgram();
}

void ProgramDialog::on_spinBoxDirectProgram_valueChanged(int)
{
    updateBankPageProgram();
}

void ProgramDialog::updateBankPageProgram()
{
    m_program.valid = true;
    if (ui->buttonBankA->isChecked())
    {
        m_program.bank = 0;
    }
    else if (ui->buttonBankB->isChecked())
    {
        m_program.bank = 1;
    }
    else if (ui->buttonBankC->isChecked())
    {
        m_program.bank = 2;
    }
    else if (ui->buttonBankD->isChecked())
    {
        m_program.bank = 3;
    }
    else
    {
        m_program = Program::INVALID;
    }

    m_program.program = (ui->spinBoxPage->value() - 1) * NUM_PROGRAM_BUTTONS;
    if (ui->spinBoxPage->value() == 0)
    {
        m_program = Program::INVALID;
    }

    if (ui->buttonProgram1->isChecked())
    {
        m_program.program += 0;
    }
    else if (ui->buttonProgram2->isChecked())
    {
        m_program.program += 1;
    }
    else if (ui->buttonProgram3->isChecked())
    {
        m_program.program += 2;
    }
    else if (ui->buttonProgram4->isChecked())
    {
        m_program.program += 3;
    }
    else if (ui->buttonProgram5->isChecked())
    {
        m_program.program += 4;
    }
    else
    {
        m_program = Program::INVALID;
    }

    ui->buttonNoProgram->setChecked( !m_program.valid );
}

void ProgramDialog::hideEvent(QHideEvent* e)
{
    if (ui->buttonNoProgram->isChecked())
    {
        m_program = Program::INVALID;
    }
    QDialog::hideEvent( e );
}
