#ifndef PDFPAGESIZEWRAPPER_H
#define PDFPAGESIZEWRAPPER_H

#include <QPagedPaintDevice>

int pageSizeToInt( QPagedPaintDevice::PageSize s );
QPagedPaintDevice::PageSize intToPageSize( int s );

#endif // PDFPAGESIZEWRAPPER_H
