#include "player.h"
#include "global.h"

Player::Player()
{
}

Player::~Player()
{
    if (m_audioOutput)
    {
        m_audioOutput->stop();
        delete m_audioOutput;
    }
}

void Player::stop()
{
    if (m_audioOutput)
    {
        m_audioOutput->stop();
        seek(0);
    }
    m_buffer.stop();
}

void Player::open( const QString &filename )
{
    stop();
    if (m_audioOutput)
    {
        delete m_audioOutput;
    }

    m_buffer.open( filename );

    m_audioOutput = new QAudioOutput( QAudioDeviceInfo::defaultOutputDevice(), m_buffer.audioFormat() );
    m_audioOutput->setNotifyInterval( 200 );    // sync

    connect( m_audioOutput, SIGNAL(notify()), this, SLOT(setCurrentPosition()) );
}

void Player::play()
{
    if (m_audioOutput)
    {
        m_audioOutput->start( &m_buffer.buffer() );
    }
    else
    {
        qWarning() << "no audioOutput";
    }
}

void Player::pause()
{
    if (m_audioOutput)
    {
        m_audioOutput->stop();
    }
    else
    {
        qWarning() << "no audioOutput";
    }
}

void Player::seek()
{
    if (m_audioOutput)
    {
        blockSignals(true);
        m_audioOutput->stop();
        m_buffer.decode( m_pitch, m_tempo, m_offset );
        m_audioOutput->start( &m_buffer.buffer() );
        blockSignals(false);
    }
    else
    {
        qWarning() << "no audioOutput";
    }
}

void Player::seek(double pitch, double tempo, double second)
{
    m_pitch = pitch;
    m_tempo = tempo;
    seek( second );
}

void Player::seek(double second)
{
    m_offset = second / m_tempo;
    seek();
}

double Player::duration() const
{
    return m_buffer.duration();
}











