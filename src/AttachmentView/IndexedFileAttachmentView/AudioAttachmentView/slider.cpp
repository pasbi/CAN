#include "slider.h"
#include "global.h"
#include "Attachments/AudioAttachment/sectionsmodel.h"

#include <QPainter>
#include <QTimer>

const double Slider::SLIDER_MULTIPLIER = 1000;

Slider::Slider(QWidget *parent) :
    QSlider(parent)
{
    connect( this, &QSlider::valueChanged, [this](int value)
    {
        double value_d = value / SLIDER_MULTIPLIER;
        emit valueChanged( value_d );
    });

    m_checkTurnTimer = new QTimer( this );
    connect( m_checkTurnTimer, SIGNAL(timeout()), this, SLOT(checkTurns()) );
    m_checkTurnTimer->start( 10 );
}

double Slider::value() const
{
    return QSlider::value() / SLIDER_MULTIPLIER;
}

void Slider::setValue( double value )
{
    QSlider::setValue( SLIDER_MULTIPLIER * value );
}

void Slider::setMaximum( double max )
{
    assert( SLIDER_MULTIPLIER * max >= 0 );
    QSlider::setMaximum( SLIDER_MULTIPLIER * max );
    QSlider::setMinimum(0);
}

double Slider::maximum() const
{
    return QSlider::maximum() / SLIDER_MULTIPLIER;
}

void Slider::setLeftIndicator( double value )
{
    m_leftIndicator = value;
    update();
}

void Slider::clearIndicators()
{
    setLeftIndicator( -1 );
}

void drawHandle(QPainter& painter, int pos)
{
    int width = 15;
    int height = 15;
    QPainterPath path;
    path.addEllipse(QRectF(pos -width/2, -height/2, width, height-1));
    painter.fillPath(path, QColor(255, 128, 0, 255));

    QPen pen = painter.pen();
    pen.setColor(QColor(255, 128, 0, 255).lighter());
    painter.setPen(pen);
    painter.drawEllipse(QRectF(pos-width/2+2,-height/2+2,width-5,height-5));
}

void drawRange(QPainter& painter, int start, int width, int pos)
{
    QPainterPath path;
    int height = 8;
    int r = 2;

    path.addRoundedRect(QRectF(QPointF(start, -height/2), QSizeF(width, height)), r, r );
    QLinearGradient gradient(QPointF(start, 0), QPointF(width+start, 0));
    gradient.setColorAt(qMax((double) (pos -start - 1) / width, 0.0), QColor(255, 128, 0, 255));
    gradient.setColorAt(qMin((double) (pos -start + 1) / width, 1.0), QColor(0, 0, 0, 0));
    painter.fillPath(path, gradient);

    QPen pen;
    pen.setWidthF(1.5);
    pen.setColor(QColor(80, 80, 80));
    painter.setPen(pen);
    painter.drawRoundedRect(QRectF(QPointF(start, -height/2), QSizeF(width, height)), r, r );
}

void drawOpenSection(QPainter& painter, int pos)
{
    int height = 10;
    {
        int gradientStart = pos - 2;
        int gradientEnd   = pos + 2;
        int gradientWidth = gradientEnd - gradientStart;
        QLinearGradient gradient(QPointF(gradientStart, 0), QPointF(gradientEnd, 0));
        gradient.setColorAt(0, QColor(0, 0, 0, 0));
        gradient.setColorAt(0.5, Qt::darkGray);
        gradient.setColorAt(1, QColor(0, 0, 0, 0));
        painter.fillRect(QRectF(gradientStart,  4,        gradientWidth, height), gradient);
        painter.fillRect(QRectF(gradientStart, -4-height, gradientWidth, height), gradient);
    }
}

void drawCompleteSection(QPainter& painter, int start, int end)
{
    int height = 10;
    {
        int gradientStart = start;
        int gradientEnd   = end;
        int gradientWidth = gradientEnd - gradientStart;
        QLinearGradient gradient(QPointF(gradientStart, 0), QPointF(gradientEnd, 0));
        gradient.setColorAt(0, QColor(255, 128, 0));
        gradient.setColorAt(0.5, QColor(255, 128, 0).lighter());
        gradient.setColorAt(1, QColor(255, 128, 0));
        painter.fillRect(QRectF(gradientStart,  4,        gradientWidth, height), gradient);
        painter.fillRect(QRectF(gradientStart, -4-height, gradientWidth, height), gradient);
    }
}

void Slider::paintEvent(QPaintEvent *ev)
{
    Q_UNUSED(ev);

    QPainter painter(this);
    painter.setRenderHint(QPainter::HighQualityAntialiasing);
    painter.translate(0, height()/2);

    const int handleWidth = 26;
    const int lMargin = handleWidth / 2;
    const int rMargin = handleWidth / 2;
    const int handlePos = lMargin + (width() - lMargin - rMargin) * (double) value() / maximum();

    drawRange(painter, lMargin, width() - rMargin - lMargin, handlePos);

    if (m_leftIndicator >= 0)
    {
        const double relPos = (m_leftIndicator - minimum()) / (maximum() - minimum());
        const int pos = relPos * (width() - lMargin - rMargin) + lMargin;
        drawOpenSection(painter, pos);
    }
    else if  (m_section)
    {
        const double relPosL = (m_section->begin() - minimum()) / (maximum() - minimum());
        const double relPosR = (m_section->end()   - minimum()) / (maximum() - minimum());

        const int posL = relPosL * (width() - lMargin - rMargin) + lMargin;
        const int posR = relPosR * (width() - lMargin - rMargin) + lMargin;

        drawCompleteSection(painter, posL, posR);
    }
    drawHandle(painter, handlePos);
}

void Slider::setSection(const Section *section)
{
    m_section = section;
    update();
}

void Slider::checkTurns()
{
    if (m_section)
    {
        if (value() > m_section->end())
        {
            setValue( m_section->begin() );
            emit valueChanged( m_section->begin() );
        }

        if (value() < m_section->begin())
        {
            setValue( m_section->begin() );
            emit valueChanged( m_section->begin() );
        }
    }
}

// it makes no sense to set the minimum since it shall always be zero.
// override the int-based range functions in order to avoid accidentaly misuse.
void Slider::setRange(int min, int max)
{
    Q_UNUSED(min);
    Q_UNUSED(max);
    UNIMPLEMENTED
}

void Slider::setMinimum(int min)
{
    Q_UNUSED(min);
    UNIMPLEMENTED
}

void Slider::setMaximum(int max)
{
    Q_UNUSED(max);
    UNIMPLEMENTED
}
