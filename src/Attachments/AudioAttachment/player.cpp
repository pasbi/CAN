#include "player.h"
#include "global.h"

Player* Player::m_activePlayer = nullptr;

Player::Player()
{
}

Player::~Player()
{
    if (m_audioOutput)
    {
        m_audioOutput->stop();
        delete m_audioOutput;
        m_audioOutput = nullptr;
    }
}

void Player::stop()
{
    if (m_audioOutput)
    {
        if (m_audioOutput->state() != QAudio::StoppedState)
        {
            //m_offset = 0;
            m_audioOutput->stop();
            m_audioOutput->reset();
            m_buffer.stop();
            seek(0);
            emit stopped();
            m_activePlayer = nullptr;
        }
    }
}

void Player::open( const QString &filename )
{
    stop();
    if (m_audioOutput)
    {
        delete m_audioOutput;
        m_audioOutput = nullptr;
    }

    m_buffer.open( filename );

    m_audioOutput = new QAudioOutput( QAudioDeviceInfo::defaultOutputDevice(), m_buffer.audioFormat() );
    m_audioOutput->setNotifyInterval(11);
    connect(m_audioOutput, SIGNAL(notify()), this, SIGNAL(notify()));
    connect(m_audioOutput, &QAudioOutput::stateChanged, [this](QAudio::State state){
        if (state == QAudio::IdleState)
        {
            stop();
        }
    });
    seek(0);
}

void Player::start()
{
    if (m_audioOutput)
    {
        if (m_audioOutput->state() != QAudio::ActiveState)
        {
            if (m_activePlayer)
            {
                m_activePlayer->stop();
            }
            m_activePlayer = this;
            m_audioOutput->start( &m_buffer.buffer() );
            emit started();
        }
    }
}

void Player::pause()
{
    // save the position into offset. Else this information will be lost when stopping m_audioOutput.
    // pause shall keep the position, unlike stop.
    if (m_audioOutput)
    {
        if (m_audioOutput->state() != QAudio::StoppedState)
        {
            m_offset = position();
            m_audioOutput->stop();
            emit paused();
        }

    }
}

void Player::seek()
{
    if (m_audioOutput)
    {
        m_offset = position();
        blockSignals(true);
        bool wasActive = (m_audioOutput->state() == QAudio::ActiveState);
        m_audioOutput->stop();
        m_buffer.decode( m_pitch, m_tempo, m_offset );
        if (wasActive)
        {
            m_audioOutput->start( &m_buffer.buffer() );
        }
        blockSignals(false);
    }
}

void Player::setPitch(double pitch)
{
    if (m_pitch != pitch)
    {
        m_pitch = pitch;
        emit pitchChanged();
    }
}

void Player::setTempo(double tempo)
{
    if (m_tempo != tempo)
    {
        m_tempo = tempo;
        emit tempoChanged();
    }
}


void Player::seek(double second)
{
    bool active = (m_audioOutput->state() == QAudio::ActiveState);
    if (position() != second || m_buffer.buffer().size() == 0)
    {
        m_offset = second;
        m_audioOutput->reset();
        seek();
        if (active)
        {
            start();
        }
        emit notify();
    }
}

double Player::duration() const
{
    return m_buffer.duration();
}


void Player::setVolume(double volume)
{
    if (m_audioOutput && volume != m_audioOutput->volume())
    {
        m_audioOutput->setVolume(volume);
        emit volumeChanged();
    }
}

double Player::volume() const
{
    if (m_audioOutput)
    {
        return m_audioOutput->volume();
    }
    else
    {
        return 0;
    }
}

bool Player::isPlaying() const
{
    if (m_audioOutput)
    {
        return m_audioOutput->state() == QAudio::ActiveState;
    }
    else
    {
        return false;
    }
}

double Player::position() const
{
    double elapsed = 0;
    if (m_audioOutput)
    {
        elapsed += m_audioOutput->elapsedUSecs() / 1000.0 / 1000.0 * m_tempo;
    }
    elapsed += m_offset;

    return elapsed;
}

void Player::setCurrentSection(Section section)
{
    if (m_currentSection != section)
    {
        m_currentSection = section;
        emit currentSectionChanged();
    }
}

Section Player::currentSection() const
{
    return m_currentSection;
}


void Player::stopActivePlayer()
{
    if (m_activePlayer)
    {
        m_activePlayer->stop();
    }
}








