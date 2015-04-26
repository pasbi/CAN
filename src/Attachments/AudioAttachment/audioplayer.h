#ifndef AUDIOPLAYER_H
#define AUDIOPLAYER_H

#include <QObject>
#include <QAudioOutput>
#include <QBuffer>
#include <QDateTime>

class AudioPlayer : public QObject
{
    Q_OBJECT
public:
    AudioPlayer( QObject* parent = 0 );
    ~AudioPlayer();

    bool setFileName( const QString & filename );
    void setNotifyInterval( int msec );
    double elapsedSec() const;
    double duration() const;
    void seek(double second );

    void setPitch( double pitch );
    void setStretch( double stretch );

    // if set, f will be called all intervall msecs during audio processing.
    void setCallback( void (*f)(float progress), int intervall );

    void interrupt();

    float pitch() const { return m_pitch; }
    float tempo() const { return m_tempo; }

public slots:
    void play();
    void stop();
    void pause();


private:
    void createAudioOutput();
    QAudioOutput* m_audioOutput = NULL;
    QByteArray m_originalData;

    QBuffer m_buffer;

    QAudioFormat m_format;
    int m_notifyInterval = 10;
    QAudioOutput createAudioOutput() const;
    double bytesPerSecond() const;

    // fill the buffer from @byte offset of m_originalData
    void processBufferFrom( qint64 byte );

    // avoid jumping into samples or between channels. Aligned position
    // marks a position at the beginning of a sample of channel #0.
    qint64 alignBufferPos( qint64 pos );

    double m_pitch = 0.0;
    double m_tempo = 1;

    qint64 currentPosInBytes() const;

    void fillBuffer();

    void runCallback(float progress);
    void (*m_callback)(float progress) = NULL;
    int m_intervall;
    qint64 m_lastCallbackTime = QDateTime::currentMSecsSinceEpoch();

    bool m_interruptRequested = false;

    QString m_filename;


signals:
    void notify();

};

#endif // AUDIOPLAYER_H
