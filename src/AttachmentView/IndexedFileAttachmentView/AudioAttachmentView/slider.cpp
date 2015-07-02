#include "slider.h"
#include "global.h"

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
    QSlider::setMaximum( SLIDER_MULTIPLIER * max );
}

double Slider::maximum() const
{
    return QSlider::maximum() / SLIDER_MULTIPLIER;
}

void Slider::setMinimum( double min )
{
    QSlider::setMaximum( SLIDER_MULTIPLIER * min );
}

double Slider::minimum() const
{
    return QSlider::minimum() / SLIDER_MULTIPLIER;
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

void Slider::paintEvent(QPaintEvent *ev)
{
    QSlider::paintEvent( ev );
    QPainter painter(this);

    const int handleWidth = 26;
    const int lMargin = handleWidth / 2;
    const int rMargin = handleWidth / 2;

    if (m_leftIndicator >= 0)
    {
        painter.save();
        QPen pen = painter.pen();
        pen.setColor( Qt::blue );
        painter.setPen( pen );

        const double relPos = (m_leftIndicator - minimum()) / (maximum() - minimum());
        const int pos = relPos * (width() - lMargin - rMargin) + lMargin;

        painter.drawLine( pos, 0, pos, height() );

        painter.restore();
    }

    if (m_section)
    {
        const double relPosL = (m_section->begin() - minimum()) / (maximum() - minimum());
        const double relPosR = (m_section->end()   - minimum()) / (maximum() - minimum());

        const int posL = relPosL * (width() - lMargin - rMargin) + lMargin;
        const int posR = relPosR * (width() - lMargin - rMargin) + lMargin;

        painter.drawLine( posL, 0, posL, height() );
        painter.drawLine( posR, 0, posR, height() );
    }
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
    }
}
