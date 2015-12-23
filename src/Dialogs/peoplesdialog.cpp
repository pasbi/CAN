#include "peoplesdialog.h"
#include "ui_peoplesdialog.h"
#include "global.h"
#include <QPushButton>
#include <QTimer>
#include <QComboBox>



QComboBox* createComboBox(const QStringList& items, const QString& currentName)
{
    QComboBox* box = new QComboBox();
    box->addItems(items);
    box->setInsertPolicy(QComboBox::NoInsert);
    if (currentName.isEmpty())
    {
        box->setCurrentIndex(0);
    }
    else
    {
        box->setCurrentText(currentName);
    }
    box->setEditable(true);
    return box;
}

PeoplesDialog::PeoplesDialog(const QStringList &allPeoples, const QStringList& activePeoples, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PeoplesDialog),
    m_activePeoples(activePeoples),
    m_allPeoples(allPeoples)
{
    ui->setupUi(this);
    for (const QString& name : activePeoples)
    {
        int n = ui->tableWidget->rowCount();
        ui->tableWidget->insertRow( n );
        ui->tableWidget->setCellWidget( n, 0, createComboBox(allPeoples, name) );
        ui->tableWidget->setCellWidget( n, 1, createRemoveButton(ui->tableWidget->cellWidget(n, 0)) );
    }
    newRow(false);

    QTimer::singleShot(0, this, SLOT( updateColumnSizes() ));
}

void PeoplesDialog::newRow(bool overwriteFormerWidgets)
{
    int n = ui->tableWidget->rowCount();
    ui->tableWidget->insertRow( n );
    if (overwriteFormerWidgets)
    {
        ui->tableWidget->setCellWidget( n - 1, 0, createComboBox(m_allPeoples, ""));
        ui->tableWidget->setCellWidget( n - 1, 1, createRemoveButton(ui->tableWidget->cellWidget(n-1, 0)) );
    }
    ui->tableWidget->setCellWidget( n, 1, new QWidget() );
    ui->tableWidget->setCellWidget( n, 0, createLastRowButton() );
}

QPushButton* PeoplesDialog::createLastRowButton()
{
    QPushButton* button = new QPushButton(QIcon(":/icons/icons/plus44.png"), "");
    connect(button, SIGNAL(clicked()), this, SLOT(newRow()));
    return button;
}


QPushButton* PeoplesDialog::createRemoveButton(QWidget* cellWidget)
{
    QPushButton* button = new QPushButton(QIcon(":/icons/icons/cross56.png"), "");
    QObject::connect(button, &QPushButton::clicked, [this, cellWidget]()
    {
        int row = -1;
        for (int i = 0; i < ui->tableWidget->rowCount(); ++i)
        {
            if (cellWidget == ui->tableWidget->cellWidget(i, 0))
            {
                row = i;
                break;
            }
        }
        ui->tableWidget->removeRow(row);
    });

    return button;
}


PeoplesDialog::~PeoplesDialog()
{
    delete ui;
}

void PeoplesDialog::accept()
{
    m_activePeoples.clear();
    for (int i = 0; i < ui->tableWidget->rowCount() - 1; ++i)
    {
        m_activePeoples << static_cast<QComboBox*>(ui->tableWidget->cellWidget(i, 0))->currentText();
    }
    m_activePeoples.removeDuplicates();
    QDialog::accept();
}

void PeoplesDialog::resizeEvent(QResizeEvent *e)
{
    updateColumnSizes();
    QDialog::resizeEvent(e);
}

void PeoplesDialog::updateColumnSizes()
{
    int width = ui->tableWidget->viewport()->width();
    int lastColumnWidth = 36;

    ui->tableWidget->setColumnWidth(0, width - lastColumnWidth);
    ui->tableWidget->setColumnWidth(1, lastColumnWidth);
}
