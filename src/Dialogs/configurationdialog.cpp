#include "configurationdialog.h"
#include "ui_configurationdialog.h"
#include <QFormLayout>
#include <QSlider>
#include "configurationwidgets.h"
#include <QPushButton>
#include <QHBoxLayout>

ConfigurationDialog::ConfigurationDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConfigurationDialog)
{
    ui->setupUi(this);
    for (const QString & key : Configurable::registerer()->configs().keys())
    {
        buildPage( key );
    }
}

ConfigurationDialog::~ConfigurationDialog()
{
    delete ui;
}


QWidget* createWidget( QWidget* parent, ConfigurationItem* item )
{
    QWidget* widget = new QWidget(parent);
    QHBoxLayout* layout = new QHBoxLayout( widget );

    ConfigurationWidget* configWidget = ConfigurationWidget::create( item );

    QPushButton* defaultButton = new QPushButton( widget );
    defaultButton->setMinimumSize( 30, 30 );
    defaultButton->setMaximumSize( 30, 30 );
    defaultButton->setIcon( QIcon(":/oldIcons/oldIcons/1411737168_POWER - RESTART.png") );
    QObject::connect( defaultButton, &QPushButton::clicked, [item, configWidget, defaultButton]()
    {
        item->restore();
        configWidget->updateEdit();
        defaultButton->setEnabled( false );
    });

    QObject::connect( configWidget, &ConfigurationWidget::valueChanged, [defaultButton, item](QVariant value)
    {
        defaultButton->setEnabled( value != item->defaultValue() );
    });
    defaultButton->setEnabled( item->actualValue() != item->defaultValue() );

    layout->addWidget( configWidget );
    layout->addWidget( defaultButton );
    layout->setStretch(0, 1);
    layout->setStretch(1, 0);

    widget->setLayout( layout );
    return widget;
}

void ConfigurationDialog::buildPage(const QString &key)
{
    Configurable* config = Configurable::registerer()->configs()[key];
    QWidget* page = new QWidget( ui->tabWidget );
    QFormLayout* layout = new QFormLayout( page );
    bool skip = true;
    for (const QString & key : config->itemKeys())
    {
        ConfigurationItem* item = config->item( key );
        if (item->options().interface() != ConfigurationItemOptions::Hidden)
        {
            layout->addRow( item->caption().append(":"), createWidget( page, item ) );
            skip = false;
        }
    }

    if (!skip)
    {
        ui->tabWidget->addTab( page, config->caption() );
        m_configurables << config;
    }
}

void ConfigurationDialog::accept()
{
    for ( Configurable* c : m_configurables )
    {
        c->apply();
    }
    QDialog::accept();
}

void ConfigurationDialog::reject()
{
    for ( Configurable* c : m_configurables )
    {
        c->reset();
    }
    QDialog::reject();
}
