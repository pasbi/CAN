#ifndef SLIDER_H
#define SLIDER_H

#include <QSlider>
#include "Attachments/AudioAttachment/sectionsmodel.h"

class Slider : public QSlider
{
    Q_OBJECT
public:
    explicit Slider(QWidget *parent = 0);

    double value() const;
    double minimum() const;
    double maximum() const;

signals:
    void valueChanged(double value);

public slots:
    void setValue( double value );
    void setMaximum( double max );
    void setMinimum( double min );
    void setRange( double min, double max ) { setMinimum(min); setMaximum(max); }

    // override int-based setters to avoid accidental calls of QSlider::setters
    void setMaximum( int max ) { setMaximum( (double) max ); }
    void setMinimum( int min ) { setMaximum( (double) min ); }
    void setRange( int min, int max ) { setRange( (double) min, (double) max ); }

    void setLeftIndicator( double value );
    void clearIndicators();

    void setSection( const Section* section );

protected:
    void paintEvent(QPaintEvent *ev);

private:
    static const double SLIDER_MULTIPLIER;
    double m_leftIndicator = -1;
    const Section* m_section = NULL;

    QTimer* m_checkTurnTimer;
private slots:
    void checkTurns();

};

#endif // SLIDER_H
