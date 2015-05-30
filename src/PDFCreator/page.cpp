#include "page.h"
#include "global.h"

const double Page::MM_INCH = 25.4;

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

double Page::dpi() const
{
    // I guess physicalDpi and logicalDpi should be isotropic and the same
    // ( pdpix = pdpiy = ldpix = ldpiy )
    // one might test this on e.g. a device with a "retina" display
    double physicalDpi = m_picture.physicalDpiX();
    if (!qFuzzyCompare( physicalDpi, m_picture.physicalDpiY() ))
    {
        qWarning() << "phyiscal dpi is anisotropic.";
    }

    double logicalDpi = m_picture.logicalDpiX();
    if (!qFuzzyCompare( logicalDpi, m_picture.logicalDpiY() ))
    {
        qWarning() << "logical dpi is anisotropic.";
    }

    if (!qFuzzyCompare( logicalDpi, physicalDpi ))
    {
        qWarning() << "logical dpi is not the same as physical dpi.";
    }

    return logicalDpi;
}


void Page::growDownMM(double mm)
{
    m_sizeInMM.setHeight( m_sizeInMM.height() + mm );
}

QSizeF Page::sizePainter() const
{
    return QSizeF( mmInPainterUnits(sizeInMM().width()), mmInPainterUnits(sizeInMM().height()) );
}


double Page::mmInPainterUnits( double mm ) const
{
    double inches = mm / MM_INCH;
    return inches * dpi();
}

double Page::painterUnitsInMM( double painter ) const
{
    double inches = painter / dpi();
    return inches * MM_INCH;
}
