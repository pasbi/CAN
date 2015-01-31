#include "progressdialog.h"
#include "ui_progressdialog.h"

ProgressDialog* ProgressDialog::m_singleton = NULL;
QTime ProgressDialog::m_timeOfLastEventLoopProcessing;

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

ProgressDialog* ProgressDialog::singleton()
{
    if (!m_singleton)
    {
        m_singleton = new ProgressDialog();
    }

    return m_singleton;
}

void ProgressDialog::show()
{
    singleton()->QDialog::show();
}

void ProgressDialog::hide()
{
    singleton()->QDialog::hide();
}


bool ProgressDialog::isCanceled()
{
    return singleton()->isHidden();
}

void ProgressDialog::processEvents()
{
    //if (m_timeOfLastEventLoopProcessing.elapsed() > 10)
    {
        QApplication::processEvents();
        m_timeOfLastEventLoopProcessing.restart();
    }
}
