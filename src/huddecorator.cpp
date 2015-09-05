#include "huddecorator.h"

#include <QWidget>
#include <QPainter>

HudDecorator::HudDecorator(QWidget* widget, int duration, QObject* parent) :
    QAbstractAnimation(parent),
    m_widget(widget),
    m_duration(duration)
{

}

void HudDecorator::paint(QPainter &painter)
{
    if (state() == QAbstractAnimation::Running)
    {
        // create the path
        QPainterPath path;
        path.addText(QPointF(), painter.font(), m_text);

        // create the outline
        QPainterPathStroker stroker;
        stroker.setJoinStyle(Qt::RoundJoin);
        stroker.setWidth(5);
        stroker.createStroke(path);
        QPainterPath outline = stroker.createStroke(path);

        // create the colors
        int c = (1 - (double) currentTime() / duration()) * 255;
        QColor outlineColor = QColor(255, 255, 255, c);
        QColor color = QColor(0, 0, 0, c);

        // paint the stuff
        painter.save();
        painter.setRenderHint(QPainter::HighQualityAntialiasing);
        painter.translate( m_widget->width() / 2 - path.boundingRect().width() / 2,
                           m_widget->height()    - path.boundingRect().height() - 50 );
        painter.fillPath(outline, outlineColor);
        painter.fillPath(path, color);
        painter.restore();
    }
}

void HudDecorator::updateCurrentTime(int currentTime)
{
    Q_UNUSED(currentTime);
    m_widget->update();
}


int HudDecorator::duration() const
{
    return m_duration;
}

void HudDecorator::setText(QString text)
{
    if (m_text != text)
    {
        stop();
        start();
        m_text = text;
    }
}
