#include "progressdialog.h"
#include "ui_progressdialog.h"

ProgressDialog::ProgressDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ProgressDialog)
{
    ui->setupUi(this);
    setModal(true);
    connect(ui->buttonBox, SIGNAL(rejected()), this, SLOT(hide()));
    m_timeOfLastEventLoopProcessing.start();
}

ProgressDialog::~ProgressDialog()
{
    delete ui;
}

void ProgressDialog::processEvents()
{
    //if (m_timeOfLastEventLoopProcessing.elapsed() > 10)
    {
        QApplication::processEvents();
        m_timeOfLastEventLoopProcessing.restart();
    }
}
