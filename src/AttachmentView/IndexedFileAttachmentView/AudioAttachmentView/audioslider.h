#ifndef AUDIOSLIDER_H
#define AUDIOSLIDER_H

#include <QWidget>
#include <QTimer>
#include "Attachments/AudioAttachment/section.h"

namespace Ui {
class AudioSlider;
}

class AudioSlider : public QWidget
{
    Q_OBJECT

public:
    explicit AudioSlider(QWidget *parent = 0);
    ~AudioSlider();

    double value() const;
    void setTempo( double tempo );
    void setPosition( double pos );
    double maximum() const;

public slots:
    void setMaximum( double max );

    void setLeftIndicator( double value );
    void clearIndicators();

    void setSection(Section section );

    void stop();

signals:
    void valueChanged(double);
    void paused();


private:
    Ui::AudioSlider *ui;
    double m_tempo = 1;

};

#endif // AUDIOSLIDER_H
