#include "audioslider.h"
#include "ui_audioslider.h"
#include "global.h"


AudioSlider::AudioSlider(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AudioSlider)
{
    ui->setupUi(this);

    connect( ui->slider,                SIGNAL( valueChanged(double) ), this, SIGNAL( valueChanged(double) )    );
    connect( ui->slider,                SIGNAL( valueChanged(double) ), this, SLOT( setValue(double) )          );
    connect( ui->doubleSpinBoxLeft,     SIGNAL( valueChanged(double) ), this, SIGNAL( valueChanged(double) )    );
    connect( ui->doubleSpinBoxLeft,     SIGNAL( valueChanged(double) ), this, SLOT( setValue(double) )          );
    connect( ui->doubleSpinBoxRight,    SIGNAL( valueChanged(double) ), this, SIGNAL( valueChanged(double) )    );
    connect( ui->doubleSpinBoxRight,    SIGNAL( valueChanged(double) ), this, SLOT( setValue(double) )          );

}

AudioSlider::~AudioSlider()
{
    delete ui;
}

double AudioSlider::value() const
{
    return ui->doubleSpinBoxLeft->value();
}

void AudioSlider::setValue(double value)
{
    double remaining = ui->doubleSpinBoxRight->maximum() - value;

    ui->doubleSpinBoxLeft->blockSignals( true );
    ui->doubleSpinBoxRight->blockSignals( true );
    ui->slider->blockSignals( true );

    ui->doubleSpinBoxLeft->setValue( value );
    ui->doubleSpinBoxRight->setValue( remaining );
    ui->slider->setValue( value );

    ui->doubleSpinBoxLeft->blockSignals( false );
    ui->doubleSpinBoxRight->blockSignals( false );
    ui->slider->blockSignals( false );
}

void AudioSlider::setMaximum(double max)
{
    ui->doubleSpinBoxLeft->blockSignals( true );
    ui->doubleSpinBoxRight->blockSignals( true );
    ui->slider->blockSignals( true );

    ui->doubleSpinBoxLeft->setMaximum( max );
    ui->doubleSpinBoxRight->setMaximum( max );
    ui->slider->setMaximum( max );
    ui->slider->setValue( qMin(ui->slider->maximum(), (double) ui->slider->value()) );
    ui->doubleSpinBoxLeft->setValue( qMin(max, ui->doubleSpinBoxLeft->value()));
    ui->doubleSpinBoxRight->setValue( max - ui->doubleSpinBoxLeft->value() );

    ui->doubleSpinBoxLeft->blockSignals( false );
    ui->doubleSpinBoxRight->blockSignals( false );
    ui->slider->blockSignals( false );
}

void AudioSlider::setLeftIndicator( double value )
{
    ui->slider->setLeftIndicator( value );
}

void AudioSlider::clearIndicators()
{
    ui->slider->clearIndicators();
}

void AudioSlider::setSection(const Section *section)
{
    ui->slider->setSection( section );
}
