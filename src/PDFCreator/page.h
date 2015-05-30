#ifndef PAGE_H
#define PAGE_H

#include <QPicture>
#include <QPainter>
#include <QPageSize>
#include "global.h"

class Page
{
public:
    ENUM( Flag, NothingSpecial, SongStartsHere, TableOfContentsStartsHere, ExistingPage  )
    typedef QFlags<Flag> Flags;

    Page(QSizeF baseSizeInMM, Flags flags = NothingSpecial );
    ~Page();


    QPainter& painter() { return m_painter; }

    /**
     * @brief picture returns the picture that was painted on.
     *  The recording stops and cannot be continued.
     * @return
     */
    const QPicture &picture();

    /**
     * @brief sizeInMM
     * @return the size of the page in mm
     */
    QSizeF sizeInMM() const { return m_sizeInMM; }

    /**
     * @brief sizePainter
     * @return the size of the page in Painter units.
     */
    QSizeF sizePainter() const;

    void growDown( double mm );

    double dpiX() const;
    double dpiY() const;

    static const double MM_INCH;
    static const double INCH_MM;


    Flags flags() const { return m_flags; }


private:
    QPicture m_picture;
    QPainter m_painter;
    QSizeF m_sizeInMM;
    Flags m_flags;
};

#endif // PAGE_H
