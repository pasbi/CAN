#include "picturepainter.h"
#include "global.h"

PicturePainter::PicturePainter(Flag flags) :
    QPainter(),
    m_flags( flags )
{
    QPainter::begin( this );
}

PicturePainter::~PicturePainter()
{
    if (QPainter::isActive())
    {
        QPainter::end();
    }
}

void PicturePainter::stop()
{
    end();
}
