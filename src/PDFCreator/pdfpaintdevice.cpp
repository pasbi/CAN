#include "pdfpaintdevice.h"
#include "global.h"
#include "picturepainter.h"

const double PDFPaintDevice::UNIT = 100;

PDFPaintDevice::PDFPaintDevice(const QString& title, const QPdfWriter::PageSize pageSize) :
    m_title( title.simplified() ),  // title cannot display \n etc.
    m_pageSize( pageSize )
{
    // create a dummy writer to determine pagesize
    QPdfWriter writer("");
    writer.setPageSize( m_pageSize );
    m_size = writer.pageSizeMM();
}

PDFPaintDevice::~PDFPaintDevice()
{
    qDeleteAll( m_pages );
    m_pages.clear();
}

PicturePainter* PDFPaintDevice::newPicturePainter(PicturePainter::Flag flag ) const
{
    PicturePainter* pp = new PicturePainter( flag );
    pp->QPainter::scale( scale(), scale() );
    pp->QPainter::setFont( m_defaultFont );
    return pp;
}


void PDFPaintDevice::activatePage(int n)
{
    assert(n >= 0 && n < m_pages.size());
    m_currentPainter = m_pages[n];
}

void PDFPaintDevice::insertEmptyPage(int i, PicturePainter::Flag flag)
{
    m_pages.insert( i, newPicturePainter( flag ) );
    activatePage( i );
}

QPainter *PDFPaintDevice::painter() const
{
    return m_currentPainter;
}

void PDFPaintDevice::save(const QString &filename) const
{
    QPdfWriter writer(filename);
    writer.setPageSize( m_pageSize );
    writer.setTitle( m_title );
    QPainter pdfwriterPainter( &writer );

    int i = 0;
    for ( PicturePainter* pp : m_pages)
    {
        pp->stop();
        if (i++ != 0)
        {
            assert( writer.newPage() );
        }
        pdfwriterPainter.drawPicture( QPoint(), *pp );
    }
}

double PDFPaintDevice::scale() const
{
    double s = qMin( m_size.width(), m_size.height() );
    s *= 3.63;
    s /= UNIT;
    return s;
}

int PDFPaintDevice::currentPageNumber() const
{
    if (painter())
    {
        int i = 0;
        for (const QPicture* picture : m_pages)
        {
            if (picture == painter()->device())
            {
                return i;
            }
            else
            {
                i++;
            }
        }
    }
    return -1;
}

PicturePainter* PDFPaintDevice::currentPage() const
{
    return m_currentPainter;
}

void PDFPaintDevice::setDefaultFont(const QFont &font)
{
    m_defaultFont = font;
    if (painter())
    {
        painter()->setFont( font );
    }
}

void PDFPaintDevice::nextPage(PicturePainter::Flag flag)
{
    insertEmptyPage( currentPageNumber() + 1, flag );
}
