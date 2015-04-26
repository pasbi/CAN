#include "audioplayer.h"
#include <QDebug>
#include <QFile>
#include "soundtouch/SoundTouch.h"

bool decode( const QString& filename, QAudioFormat& format, QByteArray& data );

AudioPlayer::AudioPlayer(QObject *parent) :
    QObject( parent )
{
    m_buffer.open( QIODevice::ReadWrite );
}

AudioPlayer::~AudioPlayer()
{
    // do not send notify signal!
    blockSignals( true );
    stop();
    blockSignals( false );
}

void AudioPlayer::pause()
{
    if (m_audioOutput)
    {
        m_audioOutput->stop();
    }
}

void AudioPlayer::play()
{
    if (!m_audioOutput)
    {
        qWarning() << "unable to play.";
        return;
    }

    m_audioOutput->start( &m_buffer );
}

void AudioPlayer::stop()
{
    if (m_audioOutput)
    {
        m_audioOutput->stop();
        seek( 0 );
        emit notify();
    }
}

bool AudioPlayer::setFileName(const QString &filename)
{
    if (m_filename == filename)
    {
        return true;
    }

    m_filename = filename;
    delete m_audioOutput;
    m_audioOutput = NULL;

    bool success = true;

    m_originalData.clear();
    success &= decode( filename, m_format, m_originalData );   // todo decode should work on a buffer to a buffer and should be non-blocking!

    if (success)
    {
        if (m_format.isValid())
        {
            createAudioOutput();
        }
        else
        {
            qWarning() << "invalid audio format: " << m_format;
            success = false;
        }
    }

    fillBuffer();

    return success;
}

void AudioPlayer::setNotifyInterval(int msec)
{
    m_notifyInterval = msec;
    if (m_audioOutput)
    {
        m_audioOutput->setNotifyInterval( msec );
    }
}

void AudioPlayer::createAudioOutput()
{
    // create audio output
    m_audioOutput = new QAudioOutput( QAudioDeviceInfo::defaultOutputDevice(), m_format );

    // establish connections
    connect( m_audioOutput, SIGNAL(notify()), this, SIGNAL(notify()) );

    // configure
    m_audioOutput->setNotifyInterval( m_notifyInterval );
}

double AudioPlayer::bytesPerSecond() const
{
    return m_format.sampleSize() / 8 * m_format.channelCount() * m_format.sampleRate() / m_tempo;
}

double AudioPlayer::elapsedSec() const
{
    if (!m_audioOutput)
    {
        return -1;
    }
    else
    {
        return m_buffer.pos() / bytesPerSecond();
    }
}

double AudioPlayer::duration() const
{
    if (!m_audioOutput)
    {
        return -1;
    }
    else
    {                                                       // bytesPerSecond() actually contains m_tempo factor
                                                            // for some weird reason, it must be mentioned here again
        return m_buffer.buffer().length() / bytesPerSecond();
    }
}

void AudioPlayer::seek(double second)
{
    qint64 byte = alignBufferPos( second * bytesPerSecond() );

    if ( byte * m_tempo < m_originalData.length() - (m_tempo * 100) )
    {
        if (!m_buffer.seek( byte ))
        {
            qWarning() << "seek failed";
        }
    }
}

qint64 AudioPlayer::alignBufferPos(qint64 pos)
{
    qint64 sampleSize = m_format.channelCount() *  m_format.sampleSize();
    return pos - (pos % sampleSize);
}

qint64 AudioPlayer::currentPosInBytes() const
{
    return m_buffer.pos();
}

void AudioPlayer::setStretch(double stretch)
{
    if (m_tempo != stretch)
    {
        stop();
        m_tempo = stretch;
        fillBuffer();
    }
}

void AudioPlayer::setPitch(double pitch)
{
    if (m_pitch != pitch)
    {
        stop();
        m_pitch = pitch;
        fillBuffer();
    }
}

void AudioPlayer::setCallback( void(*f)(float progress), int intervall)
{
    m_callback = f;
    m_intervall = intervall;
}


extern "C"
{
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavutil/avutil.h>
}

QByteArray interleave( const QByteArray a, const QByteArray b, int grain)
{
    QByteArray interleaved;
    Q_ASSERT( a.length() == b.length() );
    int n = a.length();

    for (int i = 0; i < n; i += grain)
    {
        for (int j = i; j < i + grain; ++j)
        {
            interleaved.append( a.at(j) );
        }
        for (int j = i; j < i + grain; ++j)
        {
            interleaved.append( b.at(j) );
        }
    }

    return interleaved;
}


QAudioFormat audioFormat( const int channels, const int sampleRate, AVSampleFormat sampleFormat )
{
    QAudioFormat format;
    format.setChannelCount( channels );
    format.setCodec( "audio/pcm" );
    switch (sampleFormat)
    {
    case AV_SAMPLE_FMT_U8:          ///< unsigned 8 bits
        format.setSampleSize(8);
        format.setSampleType( QAudioFormat::UnSignedInt  );
        break;
    case AV_SAMPLE_FMT_S16:         ///< signed 16 bits
        format.setSampleSize(16);
        format.setSampleType( QAudioFormat::SignedInt  );
        break;

    case AV_SAMPLE_FMT_S32:         ///< signed 32 bits
        format.setSampleSize(32);
        format.setSampleType( QAudioFormat::SignedInt  );
        break;
    case AV_SAMPLE_FMT_FLT:         ///< float
        format.setSampleSize(16);
        format.setSampleType( QAudioFormat::Float  );
        break;
    case AV_SAMPLE_FMT_DBL:         ///< double
        format.setSampleSize(32);
        format.setSampleType( QAudioFormat::Float  );
        break;

    case AV_SAMPLE_FMT_U8P:         ///< unsigned 8 bits: planar
        format.setSampleSize(8);
        format.setSampleType( QAudioFormat::UnSignedInt  );
        break;
    case AV_SAMPLE_FMT_S16P:        ///< signed 16 bits: planar
        format.setSampleSize(16);
        format.setSampleType( QAudioFormat::SignedInt  );
        break;
    case AV_SAMPLE_FMT_S32P:        ///< signed 32 bits: planar
        format.setSampleSize(32);
        format.setSampleType( QAudioFormat::SignedInt  );
        break;
    case AV_SAMPLE_FMT_FLTP:        ///< float: planar
        format.setSampleSize(16);
        format.setSampleType( QAudioFormat::SignedInt  );
        break;
    case AV_SAMPLE_FMT_DBLP:        ///< double, planar
        format.setSampleSize(32);
        format.setSampleType( QAudioFormat::SignedInt  );
        break;
    default:
        qWarning() << "codec format: " << sampleFormat << AV_SAMPLE_FMT_NONE;
        return QAudioFormat();
    }

    format.setSampleRate( sampleRate );
    return format;
}

bool decode( const QString& filename, QAudioFormat& format, QByteArray& data )
{
    // Initialize FFmpeg

    av_register_all();

    AVFrame* frame = avcodec_alloc_frame();
    if (!frame)
    {
        qWarning() << "Error allocating the frame";
        return 1;
    }

    AVFormatContext* formatContext = NULL;
    if (avformat_open_input(&formatContext, filename.toStdString().c_str(), NULL, NULL) != 0)
    {
        av_free(frame);
        qWarning() << "Error opening the file" << filename;
        return false;
    }

    if (avformat_find_stream_info(formatContext, NULL) < 0)
    {
        av_free(frame);
        avformat_close_input(&formatContext);
        qWarning() << "Error finding the stream info";
        return false;
    }

    // Find the audio stream
    AVCodec* cdc = nullptr;
    int streamIndex = av_find_best_stream(formatContext, AVMEDIA_TYPE_AUDIO, -1, -1, &cdc, 0);
    if (streamIndex < 0)
    {
        av_free(frame);
        avformat_close_input(&formatContext);
        qWarning() << "Could not find any audio stream in the file";
        return false;
    }

    AVStream* audioStream = formatContext->streams[streamIndex];
    AVCodecContext* codecContext = audioStream->codec;
    codecContext->codec = cdc;

    if (avcodec_open2(codecContext, codecContext->codec, NULL) != 0)
    {
        av_free(frame);
        avformat_close_input(&formatContext);
        qWarning() << "Couldn't open the context with the decoder";
        return false;
    }

    format = audioFormat( codecContext->channels, codecContext->sample_rate, codecContext->sample_fmt );
    if ( format.channelCount() != 1 && format.channelCount() != 2 )
    {
        av_free(frame);
        avformat_close_input(&formatContext);
        qWarning() << format.channelCount() << " Channel formats are not supported. Use Mono or Stereo.";
        return false;
    }

    int sampleSizeInBytes = format.sampleSize() / 8;


    AVPacket readingPacket;
    av_init_packet(&readingPacket);

    // Read the packets in a loop
    while (av_read_frame(formatContext, &readingPacket) == 0)
    {
        if (readingPacket.stream_index == audioStream->index)
        {
            AVPacket decodingPacket = readingPacket;

            // Audio packets can have multiple audio frames in a single packet
            while (decodingPacket.size > 0)
            {
                // Try to decode the packet into a frame
                // Some frames rely on multiple packets, so we have to make sure the frame is finished before
                // we can use it
                int gotFrame = 0;
                int result = avcodec_decode_audio4(codecContext, frame, &gotFrame, &decodingPacket);

                if (result >= 0 && gotFrame)
                {
                    decodingPacket.size -= result;
                    decodingPacket.data += result;
                    if (format.channelCount() == 1)
                    {
                        data.append( QByteArray( (char*) frame->extended_data[0], frame->linesize[0] ) );
                    }
                    else if (format.channelCount() == 2)
                    {
                        data.append( interleave( QByteArray( (char*) frame->extended_data[0], frame->linesize[0] ),
                                                 QByteArray( (char*) frame->extended_data[1], frame->linesize[0] ), sampleSizeInBytes) );
                    }
                    else
                    {
                        Q_ASSERT( false );
                    }
                }
                else
                {
                    decodingPacket.size = 0;
                    decodingPacket.data = nullptr;
                }

            }
        }

        // You *must* call av_free_packet() after each call to av_read_frame() or else you'll leak memory
        av_free_packet(&readingPacket);
    }

    if (codecContext->codec->capabilities & CODEC_CAP_DELAY)
    {
        av_init_packet(&readingPacket);
        // Decode all the remaining frames in the buffer, until the end is reached
        int gotFrame = 0;
        while (avcodec_decode_audio4(codecContext, frame, &gotFrame, &readingPacket) >= 0 && gotFrame)
        {
            // We now have a fully decoded audio frame
            if (format.channelCount() == 1)
            {
                data.append( QByteArray( (char*) frame->extended_data[0], frame->linesize[0] ) );
            }
            else if (format.channelCount() == 2)
            {
                data.append( interleave( QByteArray( (char*) frame->extended_data[0], frame->linesize[0] ),
                                         QByteArray( (char*) frame->extended_data[1], frame->linesize[0] ), sampleSizeInBytes) );
            }
            else
            {
                Q_ASSERT( false );
            }

        }
    }
    av_free(frame);
    avcodec_close(codecContext);
    avformat_close_input(&formatContext);
    return true;
}

union char2short
{
    short s;
    struct c
    {
        char c1;
        char c2;
    } c;
};

void toFloat( char c1, char c2, float& f )
{
    union char2short c2s;
    c2s.c.c1 = c1;
    c2s.c.c2 = c2;
    f = (float) c2s.s / 32768.f;      // [0,2]
//    f -= 1.f;                         // [-1,1]
}

void fromFloat( float f, char& c1, char& c2 )
{
    union char2short c2s;
//    f += 1;                 // [0, 2]
    f *= 32768.f;           // [0,65536]
    c2s.s = f;
    c1 = c2s.c.c1;
    c2 = c2s.c.c2;
}

void AudioPlayer::runCallback( float progress )
{
    qint64 currentTime = QDateTime::currentMSecsSinceEpoch();
    if (currentTime - m_lastCallbackTime > m_intervall)
    {
        m_lastCallbackTime = currentTime;
        if (m_callback)
        {
            m_callback( progress );
        }
    }
}

void AudioPlayer::interrupt()
{
    m_interruptRequested = true;
}

void AudioPlayer::fillBuffer( )
{
    if (m_pitch == 0 && m_tempo == 1)
    {
        m_buffer.buffer() = m_originalData;
    }
    else
    {
        m_interruptRequested = false;
        m_buffer.close();
        m_buffer.buffer().clear();

        soundtouch::SoundTouch processor;
        processor.setSampleRate( m_format.sampleRate() );
        processor.setChannels( m_format.channelCount() );
        processor.setPitchSemiTones((float) m_pitch);
        processor.setTempo(m_tempo);

        int sampleSize = (m_format.sampleSize() / 8);
        Q_ASSERT( sampleSize == 2 );

        const uint bufferSize = 2;
        float buffer[bufferSize];

        int n = m_format.channelCount();
        if (n == 1 || n == 2)
        {
            for (int i = 0; i < m_originalData.length(); i += sampleSize * bufferSize)
            {
                if (m_interruptRequested)
                {
                    m_pitch = 0;
                    m_tempo = 1;
                    m_buffer.buffer() = m_originalData;
                    m_interruptRequested = false;
                    return;
                }
                runCallback( (float) i / m_originalData.length() );
                for (uint j = 0; j < bufferSize; j++)
                {
                    toFloat( m_originalData.at(i + 2*j), m_originalData.at(i + 2*j + 1), buffer[j] );
                }

                processor.putSamples( buffer, bufferSize / n );  // stereo: 2 channels
                uint num = bufferSize;
                while( (num = n * processor.receiveSamples( buffer, bufferSize / n )) > 0 )
                {
                    for (uint j = 0; j < num; j++)
                    {
                        char c1, c2;
                        fromFloat( buffer[j], c1, c2 );
                        m_buffer.buffer().append( c1 );
                        m_buffer.buffer().append( c2 );
                    }
                }
            }

            processor.flush();
            uint num = bufferSize;
            while( (num = n * processor.receiveSamples( buffer, bufferSize / n )) > 0 )
            {
                for (uint j = 0; j < num; j++)
                {
                    char c1, c2;
                    fromFloat( buffer[j], c1, c2 );
                    m_buffer.buffer().append( c1 );
                    m_buffer.buffer().append( c2 );
                }
            }
        }
        else
        {
            // not supported
            qWarning() << "Formats other than mono or stereo are not supported.";
        }

        Q_ASSERT( m_buffer.open( QIODevice::ReadWrite ) );
        m_interruptRequested = false;
    }
}



































