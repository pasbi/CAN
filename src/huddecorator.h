#ifndef HUDDECORATOR_H
#define HUDDECORATOR_H

#include <QAbstractAnimation>

class QWidget;
class QPainter;
class HudDecorator : QAbstractAnimation
{
public:
    HudDecorator(QWidget* widget, int duration, QObject* parent = 0);

    void paint(QPainter& painter);


public slots:
    void setText(QString text);

private:
    void updateCurrentTime(int currentTime);
    int duration() const;
    QWidget* m_widget;
    int m_duration;
    QString m_text;
};

#endif // HUDDECORATOR_H
