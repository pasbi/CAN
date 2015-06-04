#ifndef AUDIOSLIDER_H
#define AUDIOSLIDER_H

#include <QWidget>
#include "Attachments/AudioAttachment/sectionsmodel.h"

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

public slots:
    void setMaximum( double max );

    void setValue( double value );
    void setLeftIndicator( double value );
    void clearIndicators();

//    void setSection( const Section* section );

signals:
    void valueChanged(double);


private:
    Ui::AudioSlider *ui;

};

#endif // AUDIOSLIDER_H
