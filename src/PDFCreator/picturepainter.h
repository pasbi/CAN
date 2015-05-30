#ifndef PICTUREPAINTER_H
#define PICTUREPAINTER_H

#include <QPainter>
#include <QPicture>
#include "global.h"
#include <QPageSize>


/**
 * @brief The PicturePainter class joins picture with painter.
 *  The reason for this class is, that you cannot paint twice
 *  on a picture. The second painting attempt will erase all
 *  recorded commands from the first. When each picture has its
 *  own painter, this behaviour is not crucial anymore.
 */
class PicturePainter : public QPainter, public QPicture
{
public:
    ENUM( Flag, NothingSpecial, SongStartsHere, TableOfContentsStartsHere, ExistingPage )

    /**
     * @brief PicturePainter
     * @param blank nothing can be drawn on blank pages.
     */
    explicit PicturePainter( Flag flags, QPageSize initialSize );
    ~PicturePainter();


    /**
     * @brief stop saves the recording. If the recording was stopped once,
     *  it is not possible to continue.
     */
    void stop();
    Flag flag() const { return m_flags; }

    /**
     * @brief pageSize returns the size of this page. May be grown down.
     * @return
     */
    QPageSize pageSize() const { return m_pageSize; }
    void growDown(double mm);

private:
    Flag m_flags;
    QPageSize m_pageSize;

};

#endif // PICTUREPAINTER_H
