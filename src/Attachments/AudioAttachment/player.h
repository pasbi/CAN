#ifndef PLAYER_H
#define PLAYER_H

#include <QString>
#include "buffer.h"
#include <QAudioOutput>
#include <QObject>
#include "sectionsmodel.h"
#include <QTimer>

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
    double position() const;
    double duration() const;
    double pitch() const { return m_pitch; }
    double tempo() const { return m_tempo; }

    void setSection( const Section* section = NULL ) { m_section = section; }
    const Section* currentSection() const { return m_section; }

public slots:
    void play();
    void pause();
    void stop();

private:
    Buffer m_buffer;
    double m_pitch = 0;
    double m_tempo = 1;
    double m_offset = 0;
    QAudioOutput* m_audioOutput = NULL;
    void seek();

    const Section* m_section = NULL;

    double m_currentPosition = 0;

private slots:
    double checkSectionAndGetPosition();
    void sync();
};

#endif // PLAYER_H
