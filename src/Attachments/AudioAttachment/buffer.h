#ifndef BUFFER_H
#define BUFFER_H

#include <QThread>
#include <QBuffer>
#include <QAudioFormat>
#include <soundtouch/SoundTouch.h>


extern "C"
{
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavutil/avutil.h>
}

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
    static const double FACTOR;

    void stop();

private:
    AVFormatContext* m_formatContext = NULL;
    AVStream* m_audioStream = NULL;
    AVCodecContext* m_codecContext = NULL;
    int m_streamIndex = -1;

    double m_offset = 0;

    QBuffer m_buffer;       // start audio device here on
    QString m_source;    // original data
    QAudioFormat m_audioFormat;

private:
    class Decoder : public QThread
    {
    public:
        Decoder( AVFormatContext *  formatContext,
                 AVStream*          audioStream,
                 AVCodecContext*    codecContext,
                 int                streamIndex,
                 double             pitch,
                 double             tempo,
                 double             offset,
                 QByteArray*        dest );
        ~Decoder();
        void end();
        bool isInterruptionRequested() { return m_interruptionRequested; }

    protected:
        void run();

    private:

        AVFormatContext* m_formatContext = NULL;
        AVStream* m_audioStream = NULL;
        AVCodecContext* m_codecContext = NULL;
        AVFrame* m_frame = NULL;
        int m_streamIndex = -1;

        soundtouch::SoundTouch m_soundTouch;

        QByteArray* const m_dest;
        double m_pitch;
        double m_tempo;
        double m_offset;
        bool m_interruptionRequested = false;
        void requestInterruption() { m_interruptionRequested = true; }
        bool decode();


    } * m_decoder = NULL;

};

#endif // BUFFER_H
