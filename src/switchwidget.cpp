#include "switchwidget.h"

#include <QPainter>
#include <QKeyEvent>

#include "global.h"

SwitchWidget::SwitchWidget(QWidget *parent) :
    QWidget(parent),
    m_currentIndex(0)
{
    m_songLabel = tr("Songs");
    m_eventLabel = tr("Events");
}

void SwitchWidget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    QPen pen = painter.pen();
    pen.setColor(Qt::black);
    painter.setPen(pen);

    int mid = width() / 2;

    QRect songRect = QRect(0, 0, mid, height());
    QRect eventRect = QRect(mid, 0, width() - mid, height());


    QColor songColor = QColor("crimson");
    QColor eventColor = QColor("mediumspringgreen");

    if (m_currentIndex == 1)
    {
        songColor  = QColor(Qt::red);
    }
    else
    {
        songColor  = QColor::fromHsv(songColor.hue(),
                                     songColor.saturation()  * (m_currentIndex==2 ? 0.3 : 1),
                                     songColor.value()       * (m_currentIndex==2 ? 0.3 : 1)  );
    }
    eventColor = QColor::fromHsv(eventColor.hue(),
                                 eventColor.saturation() * (m_currentIndex!=2 ? 0.3 : 1),
                                 eventColor.value()      * (m_currentIndex!=2 ? 0.3 : 1)  );

    painter.fillRect(songRect, songColor);
    painter.fillRect(eventRect, eventColor);

    pen.setColor(m_currentIndex!=2 ? Qt::black : Qt::darkGray);
    painter.setPen(pen);
    painter.drawText(songRect, Qt::AlignCenter, m_songLabel);

    pen.setColor(m_currentIndex==2 ? Qt::black : Qt::darkGray);
    painter.setPen(pen);
    painter.drawText(eventRect, Qt::AlignCenter, m_eventLabel);

    pen.setWidth(3);
    pen.setColor(Qt::black);
    painter.setPen(pen);
//    painter.drawRect(m_currentIndex ? eventRect : songRect);


}

void SwitchWidget::mousePressEvent(QMouseEvent *event)
{
    int index = -1;
    double x = (double) event->pos().x() / width();
    if (x < 0.5)
    {
        if (m_currentIndex == 0)
        {
            index = 1;
        }
        else
        {
            index = 0;
        }

    }
    else
    {
        index = 2;
    }
    if (index != m_currentIndex)
    {
        m_currentIndex = index;
        emit currentIndexChanged(index);
    }
    update();
}

int SwitchWidget::currentIndex() const
{
    return m_currentIndex;
}

void SwitchWidget::setCurrentIndex(int index)
{
    if (m_currentIndex != index)
    {
        m_currentIndex = index;
        Q_ASSERT(qBound(0, index, 2) == index);
        update();
        emit currentIndexChanged(index);
    }
}
