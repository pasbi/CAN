#include "peoplesdialog.h"
#include "ui_peoplesdialog.h"
#include "global.h"

PeoplesDialog::PeoplesDialog(const QStringList &peoples, const QBitArray &actives, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PeoplesDialog),
    m_actives(actives)
{
    ui->setupUi(this);

    assert(peoples.length() == actives.size());
    for (int i = 0; i < peoples.length(); ++i)
    {
        ui->tableWidget->insertRow(i);

        QTableWidgetItem* checkItem = new QTableWidgetItem();
        checkItem->setCheckState(actives.at(i) ? Qt::Checked : Qt::Unchecked);
        ui->tableWidget->setItem(i, 0, checkItem);

        QTableWidgetItem* nameItem = new QTableWidgetItem(peoples[i] );
        ui->tableWidget->setItem(i, 1, nameItem);
    }

}

PeoplesDialog::~PeoplesDialog()
{
    delete ui;
}

void PeoplesDialog::accept()
{
    for (int i = 0; i < m_actives.size(); ++i)
    {
        m_actives[i] = ui->tableWidget->item(i, 0)->checkState() == Qt::Checked;
    }
    QDialog::accept();
}
