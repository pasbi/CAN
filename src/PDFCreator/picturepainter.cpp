#include "picturepainter.h"
#include "global.h"

PicturePainter::PicturePainter(Flag flags, QPageSize initialSize) :
    QPainter(),
    m_flags( flags ),
    m_pageSize( initialSize )
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

QPageSize grow(const QPageSize& page, double mm)
{
    QSizeF size = page.size( QPageSize::Millimeter );
    size.setHeight( size.height() + mm );
    return QPageSize( size, QPageSize::Millimeter );
}

void PicturePainter::growDown(double mm)
{
    qDebug() << "grow beforew: " << m_pageSize.size(QPageSize::Millimeter).height();
    m_pageSize = grow( m_pageSize, mm );
    qDebug() << "grow after  : " << m_pageSize.size(QPageSize::Millimeter).height();
}
