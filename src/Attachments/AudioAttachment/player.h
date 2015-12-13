#ifndef PLAYER_H
#define PLAYER_H

#include <QObject>
#include <QString>
#include <QAudioOutput>
#include <QTimer>

#include "buffer.h"

class Player : public QObject
{
    Q_OBJECT

public:
    Player();
    ~Player();

    void open( const QString& filename );
    void seek( double pitch, double tempo, double second );
    void seek( double second );

    // position and duration are independend from tempo.
    double duration() const;
    double pitch() const { return m_pitch; }
    double tempo() const { return m_tempo; }
    double position() const;

    bool isPlaying() const;

    void setVolume(double volume);

public slots:
    void start();
    void pause();
    void stop();

signals:
    void started();
    void paused();
    void stopped();
    void notify();
    void tempoChanged();
    void pitchChanged();

public: //TODO
    Buffer m_buffer;
    double m_pitch = 0;
    double m_tempo = 1;
    double m_offset = 0;
    QAudioOutput* m_audioOutput = nullptr;
    void seek();

};
#endif // PLAYER_H
