#ifndef SLIDER_H
#define SLIDER_H

#include <QSlider>
#include "Attachments/AudioAttachment/section.h"

class Slider : public QSlider
{
    Q_OBJECT
public:
    explicit Slider(QWidget *parent = 0);

    double value() const;
    double maximum() const;

signals:
    void valueChanged(double value);

public slots:
    void setValue( double value );
    void setMaximum( double max );

    void setLeftIndicator( double value );
    void clearIndicators();

    void setSection(Section section );

protected:
    void paintEvent(QPaintEvent *ev);
    void mousePressEvent(QMouseEvent *ev);

private:
    static const double SLIDER_MULTIPLIER;
    double m_leftIndicator = -1;
    Section m_section;

    static void drawHandle(QPainter& painter, int pos);
    static void drawRange(QPainter& painter, int start, int width, int pos);


    QTimer* m_checkTurnTimer;
private slots:
    void checkTurns();


private:    // disabled functions
    void setMaximum( int max );
    void setMinimum( int min );
    void setRange( int min, int max );

};

#endif // SLIDER_H
