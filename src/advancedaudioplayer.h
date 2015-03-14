#ifndef ADVANCEDAUDIOPLAYER_H
#define ADVANCEDAUDIOPLAYER_H

#include <QMediaPlayer>
#include <QAudioProbe>
#include "soundtouch/SoundTouch.h"
#include <QMediaResource>
#include <QAudioDecoder>
#include <QAudioOutput>
#include <QBuffer>
#include <QObject>

class AdvancedAudioPlayer : QObject
{
    Q_OBJECT
public:
    AdvancedAudioPlayer();

    void setFile(const QString & filename);
    void play();

public slots:
    void readBuffer();

private:
    QString m_filename;
    QAudioDecoder* m_decoder;
    QAudioOutput* m_output;
    QByteArray m_bufferedData;
};

#endif // ADVANCEDAUDIOPLAYER_H
