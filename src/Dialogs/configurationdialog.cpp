#include "configurationdialog.h"
#include "ui_configurationdialog.h"
#include <QFormLayout>
#include <QSlider>
#include "configurationwidgets.h"
#include <QPushButton>
#include <QHBoxLayout>
#include <QLabel>
#include <QScrollArea>

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

//
QWidget* createWidget( QWidget* parent, ConfigurableItem* item )
{
    QString tooltip = item->help();
    QWidget* widget = new QWidget(parent);
    widget->setToolTip( tooltip );
    QHBoxLayout* layout = new QHBoxLayout( widget );
    layout->setContentsMargins( 0, 0, 0, 0 );

    ConfigurationWidget* configWidget = ConfigurationWidget::create( item, widget );

    QPushButton* defaultButton = new QPushButton( widget );
    defaultButton->setMaximumSize( 20, 20 );
    defaultButton->setIcon( QIcon(":/oldIcons/oldIcons/1411737168_POWER - RESTART.png") );
    defaultButton->setToolTip( ConfigurationDialog::tr("Reset value to default.") );
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
    layout->setStretch(1, 0);

    widget->setLayout( layout );
    return widget;
}

void ConfigurationDialog::buildPage(const QString &key)
{
    Configurable* config = Configurable::registerer()->configs()[key];
    QScrollArea* scrollArea = new QScrollArea( ui->tabWidget );
    scrollArea->setWidgetResizable( true );
    QWidget* page = new QWidget( scrollArea );

    QGridLayout* layout = new QGridLayout( page );

    layout->setContentsMargins( 0, 0, 0, 0 );
    int row = 0;
    for (const QString & key : config->itemKeys())
    {
        ConfigurableItem* item = config->item( key );
        if (item->options().interface() != ConfigurableItemOptions::Hidden)
        {
            QLabel* label = new QLabel(page);
            label->setText( item->caption() + ":" );
            layout->setContentsMargins( 0, 0, 0, 0 );
            layout->addWidget( label,                      row, 0 );
            layout->addWidget( createWidget( page, item ), row, 1 );

            row++;
        }
    }

    scrollArea->setWidget( page );
    if (row > 0)    // if any widget was inserted, else the page would be empty
    {
        ui->tabWidget->addTab( scrollArea, config->caption() );
        m_configurables << config;
        m_pageKeys << key;
    }
    else
    {
        delete scrollArea;
        scrollArea = NULL;
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
    QDialog::reject();
    for ( Configurable* c : m_configurables )
    {
        c->reset();
    }
}

void ConfigurationDialog::focusPage( const QString& name )
{
    int i = m_pageKeys.indexOf( name );
    if (i >= 0)
    {
        ui->tabWidget->setCurrentIndex( i );
    }
}

