#include "overlaylabel.h"
#include <QPainter>
#include "chordpatternviewer.h"
#include "overlaydecorator.h"

OverlayLabel::OverlayLabel(QWidget *parent) :
    QLabel(parent)
{
    m_overlayDecorator = new OverlayDecorator(this, 1000, this);
    m_overlayDecorator->noOverlayOnNextSetText();
}


void OverlayLabel::setLinePos(int pos)
{
    m_linePos = pos;
    update();
}

void OverlayLabel::paintEvent(QPaintEvent *e)
{
    QLabel::paintEvent(e);

    QPainter painter(this);
    painter.setPen( QColor(255, 0, 0, 100));
    if (ChordPatternViewer::config["line"].toBool())
    {
        painter.drawLine( 0, m_linePos, width(), m_linePos );
    }
    m_overlayDecorator->paint(painter);
}

void OverlayLabel::setOverlayText(const QString &text)
{
    m_overlayDecorator->setText(text);
}
