#ifndef HUDDECORATOR_H
#define HUDDECORATOR_H

#include <QAbstractAnimation>

class QWidget;
class QPainter;
class OverlayDecorator : public QAbstractAnimation
{
public:
    OverlayDecorator(QWidget* widget, int duration, QObject* parent = 0);

    void paint(QPainter& painter);
    void noOverlayOnNextSetText();

public slots:
    void setText(QString text);

private:
    void updateCurrentTime(int currentTime);
    int duration() const;   // purely virtual inherited by QAbstractAnimation
    QWidget* m_widget;
    int m_duration;
    QString m_text;

    QPoint computeOffset();

    bool m_show = true;
};

#endif // HUDDECORATOR_H
