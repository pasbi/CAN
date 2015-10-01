#include "audioslider.h"
#include "ui_audioslider.h"
#include "global.h"


AudioSlider::AudioSlider(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AudioSlider)
{
    ui->setupUi(this);
    connect( ui->slider, SIGNAL( valueChanged(double) ), this,                   SIGNAL( valueChanged(double) ) );
    connect( ui->slider, SIGNAL( valueChanged(double) ), ui->doubleSpinBoxLeft,  SLOT(   setValue(double)     ) );
    connect( ui->slider, SIGNAL( valueChanged(double) ), ui->doubleSpinBoxRight, SLOT(   setValue(double)     ) );
    connect( &m_timer,   SIGNAL(timeout()),              this,                   SLOT(   onTimerTimeout()     ) );
    m_timer.setInterval( 20 );
}

AudioSlider::~AudioSlider()
{
    delete ui;
}

double AudioSlider::value() const
{
    return ui->doubleSpinBoxLeft->value();
}

void AudioSlider::setPosition( double pos )
{
    double remaining = ui->doubleSpinBoxRight->maximum() - pos;

    ui->doubleSpinBoxLeft->blockSignals( true );
    ui->doubleSpinBoxRight->blockSignals( true );
    ui->slider->blockSignals( true );

    qDebug() << "pos = " << pos;
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

void AudioSlider::setTempo(double tempo)
{
    m_tempo = tempo;
}

void AudioSlider::play()
{
    m_timer.start();
}

void AudioSlider::pause()
{
    m_timer.stop();
}

void AudioSlider::stop()
{
    setPosition( 0 );
    m_timer.stop();
}

void AudioSlider::onTimerTimeout()
{
    double pos = ui->slider->value() + m_timer.interval() / 1000.0 * m_tempo;
    setPosition( pos );
}
