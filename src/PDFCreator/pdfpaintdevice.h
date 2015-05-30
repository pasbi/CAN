#ifndef PDFPAINTDEVICE_H
#define PDFPAINTDEVICE_H

#include <QPdfWriter>
#include "picturepainter.h"

class PDFPaintDevice
{
public:
    explicit PDFPaintDevice(const QString& title, const QPdfWriter::PageSize pageSize);
    ~PDFPaintDevice();
    void save(const QString & filename) const;

    /**
     * @brief activatePage activates the nth page. The page must exist.
     * @see PDFPaintDevice::painter()
     * @param n the index of the wanted page.
     */
    void activatePage(int n);

    /**
     * @brief nextPage inserts an empty page after current and activates it.
     */
    void nextPage( PicturePainter::Flag flag );
    int numPages( ) const { return m_pages.size(); }
    int currentPageNumber() const;
    PicturePainter* currentPage() const;

    /**
     * @brief painter returns a painter that paints on the currently active page.
     * @return
     */
    PicturePainter* painter() const;

    /**
     * @brief insertEmptyPage inserts an empty page at index i. Maybe more empty pages
     *  will be created to prevent holes.
     * @param i
     */
    void insertEmptyPage(int i , PicturePainter::Flag flag);
    QSizeF size() const { return QSizeF( UNIT, UNIT * aspectRatio() ); }
    double aspectRatio() const { return m_size.height() / m_size.width(); }
    double scale() const;
    void setDefaultFont( const QFont & font );

    void setEndlessPageSize( bool s ) { m_endlessPageSize = s; }
    QPageSize initialPageSize() const;

private:
    QList<PicturePainter*> m_pages;
    const QString m_title;
    const QPdfWriter::PageSize m_pageSize;
    QSizeF m_size;
    QFont m_defaultFont;

    PicturePainter* newPicturePainter(PicturePainter::Flag flag) const;
    PicturePainter* m_currentPainter = NULL;

    bool m_endlessPageSize = false;

    static const double UNIT;

};



#endif // PDFPAINTDEVICE_H
