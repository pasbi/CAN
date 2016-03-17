#include "filterwidget.h"
#include "ui_filterwidget.h"
#include "global.h"
#include <QLineEdit>

FilterWidget::FilterWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FilterWidget)
{
    Q_ASSERT(false);
    ui->setupUi(this);

    connect(ui->comboBox, SIGNAL(currentTextChanged(QString)), this, SIGNAL(filterChanged(QString)));
    connect(ui->pushButton, SIGNAL(clicked()), ui->comboBox, SLOT(clearEditText()));
    ui->comboBox->lineEdit()->setPlaceholderText(tr("Filter"));

    connect(ui->pushButton, &QPushButton::clicked, [this]()
    {
        ui->comboBox->setCurrentIndex(-1);
    });
}

FilterWidget::~FilterWidget()
{
    delete ui;
}

void FilterWidget::setPattern(const QString &pattern)
{
    ui->comboBox->setCurrentText(pattern);
}

void FilterWidget::hideEvent(QHideEvent *e)
{
    // reset filter when filterwidget is about to be hidden.
    emit filterChanged( "" );
    QWidget::hideEvent(e);
}
