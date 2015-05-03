#include "picturepainter.h"

PicturePainter::PicturePainter() :
    QPainter()
{
    QPainter::begin( this );
}

PicturePainter::~PicturePainter()
{
    if (isActive())
    {
        end();
    }
}

void PicturePainter::stop()
{
    end();
}
