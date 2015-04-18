#ifndef PICTUREPAINTER_H
#define PICTUREPAINTER_H

#include <QPainter>
#include <QPicture>

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
    explicit PicturePainter();
    ~PicturePainter();

    /**
     * @brief stop saves the recording. If the recording was stopped once,
     *  it is not possible to continue.
     */
    void stop();
};

#endif // PICTUREPAINTER_H
