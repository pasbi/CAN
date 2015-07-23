#ifndef PAGE_H
#define PAGE_H

#include <QPicture>
#include <QPainter>
#include <QPageSize>
#include "global.h"

class Page
{
public:
    enum Flag { NothingSpecial, SongStartsHere, TableOfContentsStartsHere, ExistingPage };
    typedef QFlags<Flag> Flags;

    Page(QSizeF baseSizeInMM, Flags flags = NothingSpecial );
    Page(QSizeF baseSizeInMM, const QString& title, Flags flags);
    ~Page();


    QPainter& painter() { return m_painter; }

    /**
     * @brief picture returns the picture that was painted on.
     *  The recording stops and cannot be continued.
     * @return
     */
    const QPicture &picture();

    QString title() const { return m_title; }

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

    void growDownMM( double mm );

    double dpi() const;

    static const double MM_INCH;

    double mmInPainterUnits( double mm ) const;
    double painterUnitsInMM( double painter ) const;


    Flags flags() const { return m_flags; }


private:
    QPicture m_picture;
    QPainter m_painter;
    QSizeF m_sizeInMM;
    Flags m_flags;
    const QString m_title;
};

#endif // PAGE_H
