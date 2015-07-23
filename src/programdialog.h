#ifndef PROGRAMDIALOG_H
#define PROGRAMDIALOG_H

#include <QDialog>
#include "program.h"

namespace Ui {
class ProgramDialog;
}

class ProgramDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ProgramDialog(QWidget *parent = 0);
    ~ProgramDialog();
    void setProgram( const Program& program );
    Program program() const { return m_program; }

private slots:
    void on_buttonBankA_clicked();
    void on_buttonBankB_clicked();
    void on_buttonBankC_clicked();
    void on_buttonBankD_clicked();
    void on_spinBoxPage_valueChanged(int);
    void on_buttonProgram1_clicked();
    void on_buttonProgram2_clicked();
    void on_buttonProgram3_clicked();
    void on_buttonProgram4_clicked();
    void on_buttonProgram5_clicked();
    void on_buttonNoProgram_clicked();
    void on_spinBoxDirectProgram_valueChanged(int);

    void updateBankPageProgram();

protected:
    void hideEvent(QHideEvent *);


private:
    Ui::ProgramDialog *ui;
    Program m_program;
};

#endif // PROGRAMDIALOG_H
