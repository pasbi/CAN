#include "progressdialog.h"
#include "ui_progressdialog.h"
#include <QThread>
#include <QTimer>
#include "global.h"


ProgressDialog::ProgressDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ProgressDialog)
{
    ui->setupUi(this);
    m_timer = new QTimer( this );
}

ProgressDialog::~ProgressDialog()
{
    delete ui;
}

void ProgressDialog::setText( const QString & text )
{
    ui->label->setText( text );
}

void ProgressDialog::setUpdateCallback(std::function<QString ()> function, int msec )
{
    assert( !m_functionSet );
    setText( function() );
    m_function = function;
    m_functionSet = true;
    m_timer->start( msec );
    connect( m_timer, &QTimer::timeout, [this]()
    {
        if (m_functionSet)
        {
            setText( m_function() );
        }
    });
}
