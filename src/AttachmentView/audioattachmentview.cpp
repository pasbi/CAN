#include "audioattachmentview.h"
#include "ui_audioattachmentview.h"

const double AudioAttachmentView::SLIDER_MULTIPLIER = 1000;

DEFN_CREATABLE( AudioAttachmentView, AttachmentView );

AudioAttachmentView::AudioAttachmentView(QWidget* parent) :
    IndexedFileAttachmentView(parent),
    ui(new Ui::AudioAttachmentView)
{
    m_audioWidget = new QWidget(this);
    ui->setupUi( m_audioWidget );
    setWidget( m_audioWidget );

    connect( ui->doubleSpinBoxElapsed,   SIGNAL(editingFinished()),     this, SLOT(on_doubleSpinBoxElapsed_editingFinished())   );
    connect( ui->doubleSpinBoxRemaining, SIGNAL(editingFinished()),     this, SLOT(on_doubleSpinBoxRemaining_editingFinished()) );
    connect( ui->doubleSpinBoxTempo,     SIGNAL(valueChanged(double)),  this, SLOT(setPitchTempo()));
    connect( ui->doubleSpinBoxPitch,     SIGNAL(valueChanged(double)),  this, SLOT(setPitchTempo()));
    connect( ui->pushButtonPlayPause,    SIGNAL(clicked(bool)),         this, SLOT(on_pushButtonPlayPause_toggled(bool))        );
    connect( ui->pushButtonStop,         SIGNAL(clicked()),             this, SLOT(on_pushButtonStop_clicked())                 );
    connect( ui->slider,                 SIGNAL(sliderMoved(int)),      this, SLOT(on_slider_sliderMoved(int))                  );
}

AudioAttachmentView::~AudioAttachmentView()
{
    on_pushButtonStop_clicked();
    delete ui;
}

void AudioAttachmentView::polish()
{
    IndexedFileAttachmentView::polish();

    AudioAttachment* a = attachment<AudioAttachment>();
    connect( &a->player(), SIGNAL(positionChanged()), this, SLOT(updateSliders()) );
    connect( &a->player(), SIGNAL(durationChanged()), this, SLOT(updateSliderRange()) );
    connect( a, SIGNAL(hashChanged(QByteArray)), this, SLOT(updateSliderRange()));
    updateSliderRange();
    ui->doubleSpinBoxPitch->setValue( player().pitch() );
    ui->doubleSpinBoxTempo->setValue( player().tempo() );
}

void AudioAttachmentView::open()
{
}

void AudioAttachmentView::on_pushButtonStop_clicked()
{
    player().stop();
    ui->pushButtonPlayPause->blockSignals( true );
    ui->pushButtonPlayPause->setChecked( false );
    ui->pushButtonPlayPause->blockSignals( false );
}

void AudioAttachmentView::on_pushButtonPlayPause_toggled(bool checked)
{
    if (checked)
    {
        player().seek( player().position() );
        player().play();
    }
    else
    {
        player().pause();
    }
}

void AudioAttachmentView::on_doubleSpinBoxElapsed_editingFinished()
{
    player().seek( ui->doubleSpinBoxElapsed->value() );
    updateSliders();
}

void AudioAttachmentView::on_doubleSpinBoxRemaining_editingFinished()
{
    player().seek( player().duration() - ui->doubleSpinBoxElapsed->value() );
    updateSliders();
}

void AudioAttachmentView::on_slider_sliderMoved(int position)
{
    player().seek( position / SLIDER_MULTIPLIER );
    updateSliders();
}


void AudioAttachmentView::updateSliders()
{
    double elapsed = player().position();
    double remaining = player().duration() - elapsed;

    ui->doubleSpinBoxElapsed->blockSignals( true );
    ui->doubleSpinBoxRemaining->blockSignals( true );
    ui->slider->blockSignals( true );

    ui->doubleSpinBoxRemaining->setValue( remaining );
    ui->doubleSpinBoxElapsed->setValue( elapsed );
    ui->slider->setValue( elapsed * SLIDER_MULTIPLIER );

    ui->doubleSpinBoxElapsed->blockSignals( false );
    ui->doubleSpinBoxRemaining->blockSignals( false );
    ui->slider->blockSignals( false );
}

void AudioAttachmentView::updateSliderRange()
{
    ui->doubleSpinBoxElapsed->blockSignals( true );
    ui->doubleSpinBoxRemaining->blockSignals( true );
    ui->slider->blockSignals( true );

    ui->doubleSpinBoxElapsed  ->setRange( 0, player().duration() );
    ui->doubleSpinBoxRemaining->setRange( 0, player().duration() );
    ui->slider->setRange( SLIDER_MULTIPLIER * 0,
                                    SLIDER_MULTIPLIER * player().duration() );
    updateSliders();

    ui->doubleSpinBoxElapsed->blockSignals( false );
    ui->doubleSpinBoxRemaining->blockSignals( false );
    ui->slider->blockSignals( false );
}


void AudioAttachmentView::setPitchTempo()
{
    player().seek( ui->doubleSpinBoxPitch->value(), ui->doubleSpinBoxTempo->value(), player().position() );
}
