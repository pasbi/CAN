#ifndef PDFPAINTDEVICE_H
#define PDFPAINTDEVICE_H

#include <QPicture>
#include <QPdfWriter>
#include <QPainter>

class PDFPaintDevice
{
public:
    explicit PDFPaintDevice(const QString& title, const QPdfWriter::PageSize pageSize);
    void save(const QString & filename) const;

    /**
     * @brief activatePage activates the nth page. If this page does not exist,
     *  it will be created. So after calling this function, numPages will be greater n.
     * @note PDFPaintDevice::painter() will paint on the active page.
     * @see PDFPaintDevice::painter()
     * @param n the index of the wanted page.
     */
    void activatePage(int n);

    /**
     * @brief nextPage activates the next page. Same as activatePage( numPages() );
     */
    void nextPage() { activatePage( numPages() ); }
    int numPages( ) const { return m_pages.size(); }

    /**
     * @brief painter returns a painter that paints on the currently active page.
     * @return
     */
    QPainter& painter() const;

    /**
     * @brief insertEmptyPage inserts an empty page at index i. Maybe more empty pages
     *  will be created to prevent holes.
     * @param i
     */
    void insertEmptyPage( int i );
    QSizeF size() const { return QSizeF( UNIT, UNIT * aspectRatio() ); }
    double aspectRatio() const { return m_size.height() / m_size.width(); }
    double scale() const;

private:
    QList<QPicture> m_pages;
    const QString m_title;
    mutable QPainter m_painter;
    const QPdfWriter::PageSize m_pageSize;
    QSizeF m_size;

    void configurePainter() const;

    static const double UNIT;
};



#endif // PDFPAINTDEVICE_H
