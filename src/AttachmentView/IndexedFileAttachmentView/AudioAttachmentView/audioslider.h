#ifndef AUDIOSLIDER_H
#define AUDIOSLIDER_H

#include <QWidget>
#include <QTimer>

namespace Ui {
class AudioSlider;
}

class Section;
class AudioSlider : public QWidget
{
    Q_OBJECT

public:
    explicit AudioSlider(QWidget *parent = 0);
    ~AudioSlider();

    double value() const;
    void setTempo( double tempo );

public slots:
    void setMaximum( double max );

    void setLeftIndicator( double value );
    void clearIndicators();

    void setSection( const Section* section );

    void pause();
    void play();
    void stop();

signals:
    void valueChanged(double);


private:
    Ui::AudioSlider *ui;
    double m_tempo = 1;
    void setPosition( double pos );
    QTimer m_timer;

private slots:
    void onTimerTimeout();

};

#endif // AUDIOSLIDER_H
