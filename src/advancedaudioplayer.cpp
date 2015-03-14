#include "advancedaudioplayer.h"

AdvancedAudioPlayer::AdvancedAudioPlayer()
{
    m_decoder = new QAudioDecoder();
    m_output = new QAudioOutput();
}

void AdvancedAudioPlayer::setFile(const QString &filename)
{
    m_filename = filename;
}

void AdvancedAudioPlayer::play()
{
    m_decoder->setSourceFilename( m_filename );
    connect( m_decoder, SIGNAL(availabilityChanged(bool)), this, SLOT(readBuffer()) );
    m_decoder->start();
}

void AdvancedAudioPlayer::readBuffer()
{
    QAudioBuffer audioBuffer = m_decoder->read();
    m_bufferedData.append( QByteArray( audioBuffer.data<char>(), audioBuffer.byteCount() ) );

    QBuffer* buffer = new QBuffer( &m_bufferedData );

    m_output->setVolume(100);
    m_output->start( buffer );

}
