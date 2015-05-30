#include "page.h"
#include "global.h"

const double Page::MM_INCH = 25.4;
const double Page::INCH_MM = 1.0 / Page::MM_INCH;

Page::Page(QSizeF baseSizeInMM, Flags flags ) :
    m_sizeInMM( baseSizeInMM ),
    m_flags( flags )
{
    m_painter.begin( &m_picture );
}

Page::~Page()
{
}

const QPicture& Page::picture()
{
    m_painter.end();
    return m_picture;
}

double Page::dpiX() const
{
    return m_picture.physicalDpiX();
}

double Page::dpiY() const
{
    return m_picture.physicalDpiY();
}

void Page::growDown(double mm)
{
    m_sizeInMM.setHeight( m_sizeInMM.height() + mm );
}

QSizeF Page::sizePainter() const
{
    QSizeF sizeInches = sizeInMM() * INCH_MM;
    return QSizeF( sizeInches.width() * dpiX(), sizeInches.height() * dpiY() );
}
