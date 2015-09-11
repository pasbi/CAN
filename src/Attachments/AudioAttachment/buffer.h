#ifndef BUFFER_H
#define BUFFER_H

#include <QThread>
#include <QBuffer>
#include <QAudioFormat>
#include "soundtouch.h"

class Buffer
{
public:
    Buffer();
    ~Buffer();
    QBuffer& buffer() { return m_buffer; }
    void open(const QString & filename);
    QAudioFormat audioFormat() const { return m_audioFormat; }

    void decode( double pitch, double tempo, double offset );

    double duration() const;
    double position() const;
    qint64 positionInBytes() const;
    static const double FACTOR;

    void stop();

private:
#ifdef HAVE_SOUNDTOUCH
    AVFormatContext* m_formatContext = NULL;
    AVStream* m_audioStream = NULL;
    AVCodecContext* m_codecContext = NULL;
#endif
    int m_streamIndex = -1;

    double m_offset = 0;

    QBuffer m_buffer;       // start audio device here on
    QString m_source;    // original data
    QAudioFormat m_audioFormat;

private:
    class Decoder : public QThread
    {
    public:
#ifdef HAVE_SOUNDTOUCH
        Decoder( AVFormatContext *  formatContext,
                 AVStream*          audioStream,
                 AVCodecContext*    codecContext,
                 int                streamIndex,
                 double             pitch,
                 double             tempo,
                 double             offset,
                 QByteArray*        dest );
#endif
        ~Decoder();
        void end();
        bool isInterruptionRequested() { return m_interruptionRequested; }


    protected:
        void run();

    private:
#ifdef HAVE_SOUNDTOUCH
        AVFormatContext* m_formatContext = NULL;
        AVStream* m_audioStream = NULL;
        AVCodecContext* m_codecContext = NULL;
        AVFrame* m_frame = NULL;
        soundtouch::SoundTouch m_soundTouch;
#endif
        int m_streamIndex = -1;


        QByteArray* m_dest;
        double m_pitch;
        double m_tempo;
        double m_offset;
        bool m_interruptionRequested = false;
        void requestInterruption() { m_interruptionRequested = true; }
        bool decode();


    } * m_decoder = NULL;

};

namespace BufferConverters
{
void charToFloat( float& f, char c1, char c2 );
void floatToChar( char& c1, char& c2, float f);
void charToFloat( float* f, const char* c, int n );
void charToFloatInterleaved( float* f, const char* rc, const char* lc, int n );
void floatToChar( char* c, const float* f, int n );
void charToFloatInterleaved2( float* f, const char* rlc, int n );
}

#endif // BUFFER_H
