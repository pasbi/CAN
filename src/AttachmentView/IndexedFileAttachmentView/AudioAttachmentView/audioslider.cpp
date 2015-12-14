#include "audioslider.h"
#include "ui_audioslider.h"
#include "global.h"


AudioSlider::AudioSlider(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AudioSlider)
{
    ui->setupUi(this);

    connect( ui->slider, SIGNAL( valueChanged(double) ), this, SIGNAL( valueChanged(double) ) );
    connect(ui->doubleSpinBoxLeft, SIGNAL(valueChanged(double)), this, SIGNAL(valueChanged(double)));
    connect(ui->doubleSpinBoxRight, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), [this](double pos){
        double elapsed = ui->doubleSpinBoxRight->maximum() - pos;
        emit valueChanged(elapsed);
    });
}

AudioSlider::~AudioSlider()
{
    delete ui;
}

double AudioSlider::value() const
{
    return ui->slider->value();
}

void AudioSlider::setPosition( double pos )
{
    double remaining = ui->doubleSpinBoxRight->maximum() - pos;

    ui->doubleSpinBoxLeft->blockSignals( true );
    ui->doubleSpinBoxRight->blockSignals( true );
    ui->slider->blockSignals( true );

    assert( pos >= 0);

    ui->doubleSpinBoxLeft->setValue( pos );
    ui->doubleSpinBoxRight->setValue( remaining );
    ui->slider->setValue( pos );

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

double AudioSlider::maximum() const
{
    return ui->doubleSpinBoxLeft->maximum();
}

void AudioSlider::setLeftIndicator( double value )
{
    ui->slider->setLeftIndicator( value );
}

void AudioSlider::clearIndicators()
{
    ui->slider->clearIndicators();
}

void AudioSlider::setSection(Section section)
{
    ui->slider->setSection( section );
}

void AudioSlider::setTempo(double tempo)
{
    m_tempo = tempo;
}


void AudioSlider::stop()
{
    setPosition( 0 );
}

