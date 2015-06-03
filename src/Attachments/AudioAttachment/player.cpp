#include "player.h"
#include "global.h"

Player::Player()
{
    const int interval = 10;
    m_timer = new QTimer(this);
    m_timer->setInterval( 10 );
    connect( m_timer, &QTimer::timeout, [this, interval]()
    {
        m_currentPosition += interval / 1000.0;
        emit positionChanged( checkSectionAndGetPosition() );
    });
}

Player::~Player()
{
    m_timer->stop();
    if (m_audioOutput)
    {
        m_audioOutput->stop();
        delete m_audioOutput;
    }
}

void Player::stop()
{
    m_timer->stop();
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

    connect( m_audioOutput, SIGNAL(notify()), this, SLOT(sync()) );

    double pos = position();

    emit positionChanged( pos );
    emit durationChanged( duration() );
}

void Player::play()
{
    if (m_audioOutput)
    {
        m_audioOutput->start( &m_buffer.buffer() );
        m_timer->start();
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
        m_timer->stop();
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
        bool timerRun = m_timer->isActive();
        if (timerRun)
        {
            m_timer->stop();
        }
        blockSignals(true);
        m_audioOutput->stop();
        m_buffer.decode( m_pitch, m_tempo, m_offset );
        m_audioOutput->start( &m_buffer.buffer() );
        blockSignals(false);
        sync();
        if (timerRun)
        {
            m_timer->start();
        }
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
    emit durationChanged( duration() );
    seek( second );
}

void Player::seek(double second)
{
    m_offset = second;
    seek();
}

double Player::duration() const
{
    return m_buffer.duration() / m_tempo;
}

double Player::position() const
{
    return m_currentPosition;
}

double Player::checkSectionAndGetPosition()
{
    double pos = position();
    if (  m_section && (pos < m_section->begin() || pos > m_section->end()) )
    {
        pos = m_section->begin();
        seek( pos);
    }
    return pos;
}


void Player::sync()
{
    // sync
    m_currentPosition = m_buffer.position();
}










