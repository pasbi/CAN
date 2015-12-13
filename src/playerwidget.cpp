#include "playerwidget.h"
#include "ui_playerwidget.h"
#include "Attachments/AudioAttachment/player.h"

PlayerWidget::PlayerWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PlayerWidget),
    m_player(nullptr)
{
    ui->setupUi(this);

    connect(ui->pushButtonPlayPause, SIGNAL(toggled(bool)), this, SLOT(setPlayPause(bool)));
    connect(ui->pushButtonStop,      SIGNAL(clicked()), this, SLOT(stop()));
    connect(ui->doubleSpinBoxPitch,  SIGNAL(valueChanged(double)), this, SLOT(updatePitchTempo()));
    connect(ui->doubleSpinBoxTempo, SIGNAL(valueChanged(double)), this, SLOT(updatePitchTempo()));
    connect(ui->slider, SIGNAL(valueChanged(double)), this, SLOT(seek(double)));


//    connect(this, SIGNAL(volumeChanged(int)), this, SLOT(updateVolume()));
}

PlayerWidget::~PlayerWidget()
{
    delete ui;
}

void reConnect(QObject* oldSender, QObject* newSender, const char* signal, QObject* receiver, const char* slot)
{
    if (oldSender)
    {
        QObject::disconnect(oldSender, signal, receiver, slot);
    }
    if (newSender)
    {
        QObject::connect(newSender, signal, receiver, slot);
    }
}

void PlayerWidget::setPlayer(Player *player)
{
    reConnect(m_player, player, SIGNAL(paused()), this, SLOT(pause()));
    reConnect(m_player, player, SIGNAL(started()), this, SLOT(start()));
    reConnect(m_player, player, SIGNAL(stopped()), this, SLOT(stop()));
    reConnect(m_player, player, SIGNAL(notify()), this, SLOT(updatePosition()));
    reConnect(m_player, player, SIGNAL(pitchChanged()), this, SLOT(setPitchTempo()));
    reConnect(m_player, player, SIGNAL(tempoChanged()), this, SLOT(setPitchTempo()));

    m_player = player;

    ui->slider->stop();

    if (m_player)
    {
        ui->slider->setMaximum( player->duration() );
        updatePosition();

        ui->doubleSpinBoxPitch->setValue( player->pitch() );
        ui->doubleSpinBoxTempo->setValue( player->tempo() );

        if (m_player->isPlaying())
        {
            setPlayPause(true);
        }
    }
    ui->doubleSpinBoxPitch->setEnabled(m_player);
    ui->doubleSpinBoxTempo->setEnabled(m_player);
    ui->slider->setEnabled(m_player);
    ui->pushButtonPlayPause->setEnabled(m_player);
    ui->pushButtonStop->setEnabled(m_player);
}

void PlayerWidget::setSection(Section section)
{
    ui->slider->setSection(section);
}

void PlayerWidget::stop()
{
#ifdef HAVE_SOUNDTOUCH
    if (m_player)
    {
        m_player->stop();
    }
    ui->pushButtonPlayPause->setChecked( false );
    ui->slider->stop();
#endif
}

void PlayerWidget::setPlayPause(bool play)
{
#ifdef HAVE_SOUNDTOUCH
    if (m_player)
    {
        if (play)
        {
            m_player->start();
        }
        else
        {
            m_player->pause();
        }
    }
#else
    Q_UNUSED( play );
#endif
}


void PlayerWidget::seek(double pos)
{
#ifdef HAVE_SOUNDTOUCH
    if (m_player)
    {
        m_player->seek( pos );
    }
#else
    Q_UNUSED( pos );
#endif
}



void PlayerWidget::updatePitchTempo()
{
#ifdef HAVE_SOUNDTOUCH
    m_player->seek( ui->doubleSpinBoxPitch->value(), ui->doubleSpinBoxTempo->value(), ui->slider->value() );
    ui->slider->setTempo( ui->doubleSpinBoxTempo->value() );
#endif
}

void PlayerWidget::setPitchTempo()
{
#ifdef HAVE_SOUNDTOUCH
    if (m_player)
    {
        ui->doubleSpinBoxPitch->setValue( m_player->pitch() );
        ui->doubleSpinBoxTempo->setValue( m_player->tempo() );
    }
#endif
}

void PlayerWidget::pause()
{
    ui->pushButtonPlayPause->setChecked(false);
    setPlayPause(false);
}

void PlayerWidget::start()
{
    ui->pushButtonPlayPause->setChecked(true);
    setPlayPause(true);
}

void PlayerWidget::updateVolume()
{
    //config.set("Muted", ui->volumeSlider->isMuted());
    //config.set("Volume", ui->volumeSlider->value());

    //TODO attachment<AudioAttachment>()->player().setVolume(double(ui->volumeSlider->value()) / ui->volumeSlider->maximum());
}

void PlayerWidget::updatePosition()
{
    ui->slider->setPosition(m_player->position());
}