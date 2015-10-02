#include "volumeslider.h"
#include <QPainter>
#include <QMouseEvent>
#include "global.h"

void VolumeSlider::drawRange(QPainter& painter, int start, int width, int pos)
{
    QPainterPath path;

    path.moveTo(QPointF(start,  -1));
    path.lineTo(QPointF(start + width, -this->height()/2));
    path.lineTo(QPointF(start + width,  this->height()/2));
    path.lineTo(QPointF(start,  2));

    QPen pen;
    pen.setWidthF(1.5);
    pen.setColor(QColor(80, 80, 80));
    painter.setPen(pen);
    painter.drawPath(path);


    int red = 255;
    int green = 127 + 128 * double(pos-start) / width;
    int blue = 0 + 255 * double(pos-start) / width;
    int alpha = 255;
    if (m_muted)
    {
        alpha = 128;
    }
    QLinearGradient gradient(QPointF(start, 0), QPointF(width+start, 0));
    gradient.setColorAt(0,                                            QColor(255, 128, 0, alpha));
    gradient.setColorAt(qMax((double) (pos -start - 1) / width, 0.0), QColor(red, green, blue, alpha));
    gradient.setColorAt(qMin((double) (pos -start + 1) / width, 1.0), QColor(0, 0, 0, 0));
    painter.fillPath(path, gradient);

    if (m_muted)
    {
        int size = 5;
        pen.setWidth(2);
        pen.setCapStyle(Qt::RoundCap);
        pen.setColor(Qt::red);
        painter.setPen(pen);
        painter.drawLine(QPointF(start + width/2 - size, - size), QPointF(start + width/2 + size, + size));
        painter.drawLine(QPointF(start + width/2 - size, + size), QPointF(start + width/2 + size, - size));
    }
}


VolumeSlider::VolumeSlider(QWidget *parent) :
    QSlider(parent)
{
    m_muted = false;
    connect(this, &QSlider::valueChanged, [this](int value){
        if (value > 0)
        {
            setMuted(false);
        }
    });
}

void VolumeSlider::setMuted(bool muted)
{
    if (m_muted != muted)
    {
        m_muted = muted;
        emit wasMuted(m_muted);

        update();

        if (QSlider::value() > 0)
        {
            emit valueChanged(value());
        }
    }
}

void VolumeSlider::paintEvent(QPaintEvent *ev)
{
    Q_UNUSED(ev);

    QPainter painter(this);
    painter.setRenderHint(QPainter::HighQualityAntialiasing);
    painter.translate(0, height()/2);

    const int handleWidth = 26;
    const int lMargin = handleWidth / 2;
    const int rMargin = handleWidth / 2;
    const int handlePos = lMargin + (width() - lMargin - rMargin) * (double) QSlider::value() / maximum();

    drawRange(painter, lMargin, width() - rMargin - lMargin, handlePos);
}

int VolumeSlider::value() const
{
    if (m_muted)
    {
        return 0;
    }
    else
    {
        return QSlider::value();
    }
}

void VolumeSlider::mousePressEvent(QMouseEvent *ev)
{
    if (ev->type() == QEvent::MouseButtonDblClick && ev->button() == Qt::LeftButton)
    {
        setMuted(!m_muted);
    }
    else
    {
        QSlider::mousePressEvent(ev);
    }
}

bool VolumeSlider::isMuted() const
{
    return m_muted;
}
