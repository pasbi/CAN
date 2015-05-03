#include "player.h"
#include <QDebug>

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
    }
    m_buffer.stop();
}

void Player::open( const QString &filename )
{
    stop();
    m_buffer.open( filename );

    m_audioOutput = new QAudioOutput( QAudioDeviceInfo::defaultOutputDevice(), m_buffer.audioFormat() );
    connect( m_audioOutput, SIGNAL(notify()), this, SIGNAL(positionChanged()) );
    emit positionChanged();
    emit durationChanged();
    m_audioOutput->setNotifyInterval( 10 );
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
        m_audioOutput->stop();
        m_buffer.decode( m_pitch, m_tempo, m_offset );
        m_audioOutput->start( &m_buffer.buffer() );
    }
}

void Player::seek(double pitch, double tempo, double second)
{
    m_pitch = pitch;
    m_tempo = tempo;
    m_offset = second;
    seek();
}

void Player::seek(double second)
{
    m_offset = second;
    seek();
}

double Player::duration() const
{
    return m_buffer.duration();
}

double Player::position() const
{
    return m_buffer.position();
}











