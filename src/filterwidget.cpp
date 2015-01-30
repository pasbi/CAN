#include "filterwidget.h"
#include "ui_filterwidget.h"
#include "global.h"

FilterWidget::FilterWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FilterWidget)
{
    ui->setupUi(this);
    connect(ui->lineEdit, SIGNAL(textChanged(QString)), this, SIGNAL(patternChanged(QString)));
    connect(ui->pushButton, SIGNAL(clicked()), ui->lineEdit, SLOT(clear()));
}

FilterWidget::~FilterWidget()
{
    delete ui;
}

void FilterWidget::setPattern(const QString &pattern)
{
    ui->lineEdit->setText(pattern);
}


void FilterWidget::setFilterProxy(QSortFilterProxyModel *proxy)
{
    if (!m_proxy)
    {
        disconnect(this, SIGNAL(patternChanged(QString)), m_proxy, SLOT(setFilterFixedString(QString)));
    }
    m_proxy = proxy;
    if (m_proxy)
    {
        connect(this, SIGNAL(patternChanged(QString)), m_proxy, SLOT(setFilterFixedString(QString)));
    }
}
