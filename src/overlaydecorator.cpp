#include "overlaydecorator.h"
#include <QWidget>
#include <QPainter>
#include "global.h"
#include <QScrollArea>
#include <QScrollBar>

OverlayDecorator::OverlayDecorator(QWidget* widget, int duration, QObject* parent) :
    QAbstractAnimation(parent),
    m_widget(widget),
    m_duration(duration)
{

}

// It seems to be impossible to overpaint QScrollArea (no idea why not)
// So we must do the painting here. This computes the offset which is caused by the scrollArea scrollings.
QPoint OverlayDecorator::computeOffset()
{
    const QObject* object = m_widget;
    const QScrollArea* scrollArea;
    while ( object && !(scrollArea = qobject_cast<const QScrollArea*>( object->parent() )) )
    {
        object = object->parent();
    }

    if (scrollArea)
    {
        QSize viewPortSize = scrollArea->viewport()->size();
        QSize thisSize = m_widget->size();
        QPoint offset = QPoint(scrollArea->horizontalScrollBar()->value(), scrollArea->verticalScrollBar()->value());

        return offset - QPoint(thisSize.width(), thisSize.height()) + QPoint(viewPortSize.width(), viewPortSize.height());
    }
    else
    {
        return QPoint();
    }
}

void OverlayDecorator::paint(QPainter &painter)
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
        painter.translate(computeOffset());

        painter.fillPath(outline, outlineColor);
        painter.fillPath(path, color);
        painter.restore();
    }
}

void OverlayDecorator::updateCurrentTime(int currentTime)
{
    Q_UNUSED(currentTime);
    m_widget->update();
}


int OverlayDecorator::duration() const
{
    return m_duration;
}

void OverlayDecorator::setText(QString text)
{
    if (m_text != text)
    {
        stop();
        m_text = text;
        if (m_show)
        {
            start();
        }
        else
        {
            m_show = true;
        }
    }
}

void OverlayDecorator::noOverlayOnNextSetText()
{
    m_show = false;
}
