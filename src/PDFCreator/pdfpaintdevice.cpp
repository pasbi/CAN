#include "pdfpaintdevice.h"
#include "global.h"
#include <QPainter>

const double PDFPaintDevice::UNIT = 100;

PDFPaintDevice::PDFPaintDevice(const QString& title, const QPdfWriter::PageSize pageSize) :
    m_title( title.simplified() ),  // title cannot display \n etc.
    m_pageSize( pageSize )
{
    // create a dummy writer to determine pagesize
    QPdfWriter writer("");
    writer.setPageSize( m_pageSize );
    m_size = writer.pageSizeMM();
    configurePainter();
}

QPicture picture()
{
    return QPicture();
}

void PDFPaintDevice::activatePage(int n)
{
    assert(n >= 0);
    if (n >= m_pages.size())
    {
        for (int i = m_pages.size(); i <= n; ++i)
        {
            m_pages.append( picture() );
        }
    }

    if (m_painter.isActive())
    {
        m_painter.end();
    }
    m_painter.begin( &m_pages[n] );
    configurePainter();
}

void PDFPaintDevice::insertEmptyPage(int i)
{
    m_pages.insert( i, picture() );
}

QPainter& PDFPaintDevice::painter() const
{
    return m_painter;
}

void PDFPaintDevice::save(const QString &filename) const
{
    // painter must be ended to store commands in qpicture.
    // save device to be able to restore it later.
    QPaintDevice* activePaintDevice = NULL;
    if (m_painter.isActive())
    {
        activePaintDevice = m_painter.device();
        m_painter.end();
    }

    QPdfWriter writer(filename);
    writer.setPageSize( m_pageSize );
    writer.setTitle( m_title );
    QPainter painter( &writer );

    INIT_LOOP;
    for ( const QPicture& picture : m_pages)
    {
        IFN_FIRST_ITERATION assert( writer.newPage() );
        painter.drawPicture( QPoint(), picture );
    }

    if (activePaintDevice)
    {
        m_painter.begin( activePaintDevice );
        configurePainter();
    }
}

double PDFPaintDevice::scale() const
{
    double s = qMin( m_size.width(), m_size.height() );
    s *= 3.63;
    s /= UNIT;
    return s;
}

void PDFPaintDevice::configurePainter() const
{
    if (m_painter.isActive() && !!m_painter.device())
    {
        painter().scale( scale(), scale() );

        QFont font = painter().font();
        font.setPointSizeF( font.pointSizeF() / scale() );
        painter().setFont( font );
    }
}

