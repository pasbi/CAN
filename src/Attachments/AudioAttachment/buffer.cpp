#include "buffer.h"
#include <QFile>
#include "global.h"

QAudioFormat audioFormat( const int channels, const int sampleRate, AVSampleFormat sampleFormat );


Buffer::Buffer()
{
    m_buffer.open( QIODevice::ReadOnly );
}

Buffer::~Buffer()
{
    if (m_decoder)
    {
        delete m_decoder;
    }
    if (m_formatContext)
    {
        avformat_free_context( m_formatContext );
    }
}

void Buffer::open(const QString &filename)
{
    // Initialize FFmpeg

    stop();
    if (m_formatContext)
    {
        avformat_free_context( m_formatContext );
        m_formatContext = NULL;
    }

    av_register_all();

    if (avformat_open_input(&m_formatContext, filename.toStdString().c_str(), NULL, NULL) != 0)
    {
        qWarning() << "Error opening the file" << filename;
        return;
    }


    if (avformat_find_stream_info(m_formatContext, NULL) < 0)
    {
        qWarning() << "Error finding the stream info";
        return;
    }


    // Find the audio stream
    AVCodec* cdc = nullptr;
    m_streamIndex = av_find_best_stream(m_formatContext, AVMEDIA_TYPE_AUDIO, -1, -1, &cdc, 0);
    if (m_streamIndex < 0)
    {
        qWarning() << "Could not find any audio stream in the file";
        return;
    }

    m_audioStream = m_formatContext->streams[m_streamIndex];
    m_codecContext = m_audioStream->codec;
    m_codecContext->codec = cdc;

    if (avcodec_open2(m_codecContext, m_codecContext->codec, NULL) != 0)
    {
        qWarning() << "Couldn't open the context with the decoder";
        return;
    }

    m_audioFormat = ::audioFormat( m_codecContext->channels, m_codecContext->sample_rate, m_codecContext->sample_fmt );
    if ( m_codecContext->channels != 1 && m_codecContext->channels != 2 )
    {
        qWarning() << m_codecContext->channels << " Channel formats are not supported. Use Mono or Stereo.";
        return;
    }
}

void Buffer::stop()
{
    if (m_decoder)
    {
        delete m_decoder;
    }
    m_decoder = NULL;
    m_buffer.buffer().clear();
    m_buffer.reset();
}

void Buffer::decode(double pitch, double tempo, double offset)
{
    stop();
    if (m_formatContext && m_audioStream && m_codecContext && m_streamIndex >= 0)
    {
        m_decoder = new Decoder( m_formatContext,
                                 m_audioStream,
                                 m_codecContext,
                                 m_streamIndex,
                                 pitch,
                                 tempo,
                                 offset,
                                 &m_buffer.buffer() );
        m_decoder->start();
        m_offset = offset;
    }
}

Buffer::Decoder::Decoder( AVFormatContext * formatContext,
                          AVStream*         audioStream,
                          AVCodecContext *  codecContext,
                          int               streamIndex,
                          double            pitch,
                          double            tempo,
                          double            offset,
                          QByteArray*       dest            ) :
    QThread( NULL ),
    m_formatContext( formatContext ),
    m_audioStream( audioStream ),
    m_codecContext( codecContext ),
    m_streamIndex( streamIndex ),
    m_dest( dest ),
    m_pitch( pitch ),
    m_tempo( tempo ),
    m_offset( offset )
{
    m_soundTouch.setChannels( m_codecContext->channels );
    m_soundTouch.setSampleRate( m_codecContext->sample_rate );
    m_soundTouch.setTempo( tempo );
    m_soundTouch.setPitchSemiTones( (float) pitch );
    m_frame = av_frame_alloc();
}

Buffer::Decoder::~Decoder()
{
    end();
    av_frame_free( &m_frame );
}

void Buffer::Decoder::run()
{
    decode();
}

void Buffer::Decoder::end()
{
    requestInterruption();
    if (!wait(2000))
    {
        terminate();
    }

    wait();
}

double Buffer::duration() const
{
    if (m_formatContext)
    {
        return m_formatContext->duration / AV_TIME_BASE;
    }
    else
    {
        return -1;
    }
}

double Buffer::position() const
{
    if (m_codecContext)
    {
        double pos = m_buffer.pos();        // pos in bytes
        pos /= 2.f;                         // pos in samples
        pos /= m_codecContext->channels;    // pos in samples per channel
        pos /= m_codecContext->sample_rate; // pos in seconds
        return pos + m_offset;
    }
    else
    {
        return -1;
    }
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


typedef union
{
    short s;
    struct
    {
        char c1;
        char c2;
    } c;
} CharShortConverter;

void charToFloat( float& f, char c1, char c2 )
{
    CharShortConverter c;
    c.c.c1 = c1;
    c.c.c2 = c2;
    f = c.s / 32768.f;
}

void floatToChar( char& c1, char& c2, float f)
{
    CharShortConverter c;
    c.s = f * 32768.f;
    c1 = c.c.c1;
    c2 = c.c.c2;
}

void charToFloat( float* f, const char* c, int n )
{
    for (int i = 0; i < n; i++)
    {
        charToFloat( f[i], c[2*i], c[2*i+1] );
    }
}

void charToFloatInterleaved( float* f, const char* rc, const char* lc, int n )
{
    for (int i = 0; i < n; i++)
    {
        charToFloat( f[2*i+0], rc[2*i], rc[2*i+1] );
        charToFloat( f[2*i+1], lc[2*i], lc[2*i+1] );
    }
}

void floatToChar( char* c, const float* f, int n )
{
    for (int i = 0; i < n; ++i )
    {
        floatToChar( c[2*i], c[2*i+1], f[i] );
    }
}


bool Buffer::Decoder::decode()
{
    AVPacket readingPacket;
    av_init_packet(&readingPacket);

    double factor = (double) m_audioStream->time_base.den / m_codecContext->time_base.num;
    if  (av_seek_frame( m_formatContext, m_streamIndex, factor * m_offset, 0 ) != 0)
    {
        qWarning() << "cannot seek to " << m_offset;
        return false;
    }

    // Read the packets in a loop
    while (av_read_frame(m_formatContext, &readingPacket) == 0)
    {
        if (isInterruptionRequested())
        {
            return false;
        }

        if (readingPacket.stream_index == m_audioStream->index)
        {
            AVPacket decodingPacket = readingPacket;

            // Audio packets can have multiple audio frames in a single packet
            while (decodingPacket.size > 0)
            {
                // Try to decode the packet into a frame
                // Some frames rely on multiple packets, so we have to make sure the frame is finished before
                // we can use it
                int gotFrame = 0;
                int result = avcodec_decode_audio4(m_codecContext, m_frame, &gotFrame, &decodingPacket);

                if (result >= 0 && gotFrame)
                {
                    decodingPacket.size -= result;
                    decodingPacket.data += result;

                    int n = m_frame->linesize[0] / 2;
                    if (m_codecContext->channels == 1)
                    {
                        float* buffer = new float[n];
                        charToFloat( buffer, (char*) m_frame->extended_data[0], n );
                        m_soundTouch.putSamples( buffer, n );
                        delete[] buffer;
                    }
                    else if (m_codecContext->channels == 2)
                    {
                        float* buffer = new float[2*n];
                        charToFloatInterleaved( buffer,
                                                (char*) m_frame->extended_data[0],
                                                (char*) m_frame->extended_data[1],
                                                n                                          );
                        m_soundTouch.putSamples( buffer, n );
                        delete[] buffer;
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

                // get samples from soundtouch
                const int BUFF_SIZE = 16;
                uint nSamples = 0;
                do
                {
                    float sampleBuffer[BUFF_SIZE];
                    int buffSizeSamples = BUFF_SIZE / m_codecContext->channels;

                    nSamples = m_codecContext->channels * m_soundTouch.receiveSamples(sampleBuffer, buffSizeSamples);

                    char charBuffer[BUFF_SIZE * 2];
                    floatToChar( charBuffer, sampleBuffer, nSamples );
                    m_dest->append( charBuffer, nSamples * 2 );
                } while (nSamples != 0);
            }
        }

        // You *must* call av_free_packet() after each call to av_read_frame() or else you'll leak memory
        av_free_packet(&readingPacket);
    }

    if (m_codecContext->codec->capabilities & CODEC_CAP_DELAY)
    {
        av_init_packet(&readingPacket);
        // Decode all the remaining frames in the buffer, until the end is reached
        int gotFrame = 0;
        while (avcodec_decode_audio4(m_codecContext, m_frame, &gotFrame, &readingPacket) >= 0 && gotFrame)
        {
            int n = m_frame->linesize[0] / 2;
            if (m_codecContext->channels == 1)
            {
                float* buffer = new float[n];
                charToFloat( buffer, (char*) m_frame->extended_data[0], n );
                m_soundTouch.putSamples( buffer, n );
                delete[] buffer;
            }
            else if (m_codecContext->channels == 2)
            {
                float* buffer = new float[2*n];
                charToFloatInterleaved( buffer,
                                        (char*) m_frame->extended_data[0],
                                        (char*) m_frame->extended_data[1],
                                        n                                          );
                m_soundTouch.putSamples( buffer, n );
                delete[] buffer;
            }
            else
            {
                Q_ASSERT( false );
            }

        }
    }



    // get last samples from soundtouch
    m_soundTouch.flush();
    const int BUFF_SIZE = 16;
    uint nSamples = 0;
    do
    {
        float sampleBuffer[BUFF_SIZE];
        int buffSizeSamples = BUFF_SIZE / m_codecContext->channels;

        nSamples = m_codecContext->channels * m_soundTouch.receiveSamples(sampleBuffer, buffSizeSamples);

        char charBuffer[BUFF_SIZE * 2];
        floatToChar( charBuffer, sampleBuffer, nSamples );
        m_dest->append( charBuffer, nSamples * 2 );
    } while (nSamples != 0);

    return true;
}
