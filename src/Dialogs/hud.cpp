#include "hud.h"
#include <QTextFormat>
#include "global.h"
#include <QPainter>

HUD::HUD(QWidget *parent) :
    QLabel(parent),
    m_duration( 80 )
{
    m_timer = new QTimer( this );
    m_timer->setInterval( 10 );
    m_timer->setSingleShot( false );
    connect( m_timer, SIGNAL(timeout()), this, SLOT(updateTransparency()) );

    setAttribute( Qt::WA_TranslucentBackground );

    updateTransparency();
}

void HUD::show()
{
    m_i = 0;
    m_timer->start();
    QLabel::show();
}

void HUD::updateTransparency()
{
    m_i++;
    if (m_i > m_duration)
    {
        m_i = 0;
        m_timer->stop();
        hide();
    }

    int alpha = 255 * ( 1 - (double) m_i / m_duration );


    QImage image( size(), QImage::Format_ARGB32 );

    QPainter painter;

    QFont font = painter.font();
    font.setPointSizeF( 20 );
    font.setWeight( QFont::Bold );

    painter.begin(&image);
    painter.setFont( font );
    painter.setPen( QColor(0, 0, 0, alpha) );
    int width = painter.fontMetrics().width( m_text );
    int height = painter.fontMetrics().height();
    width += 10;
    height += 10;
    painter.end();

    image = QImage( QSize( width, height ), QImage::Format_ARGB32 );

    painter.begin( &image );
    painter.setFont( font );
    painter.setPen( QColor(0, 0, 0, alpha) );
    image.fill( QColor(0, 0, 0, 0) );
    painter.drawText( rect(), m_text, QTextOption( Qt::AlignCenter ) );
    painter.end();
    resize( width, height );
    setPixmap( QPixmap::fromImage( image ) );

    update();
}

void HUD::setText(const QString &text)
{
    m_text = text;
}

//void HUD::paintEvent(QPaintEvent *e)
//{
//    QLabel::paintEvent(e);

//    QPainter painter(this);
//    painter.fillRect( rect(), Qt::red );

//}

void HUD::replace()
{
    if (!parentWidget())
    {
        return;
    }
    move( parentWidget()->width()/2 - width()/2, parentWidget()->height() - height() - 20 );
}
