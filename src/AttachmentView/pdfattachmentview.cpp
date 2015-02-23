#include "pdfattachmentview.h"
#include "ui_pdfattachmentview.h"
#include "Attachments/pdfattachment.h"
#include <QPainter>
#include <qmath.h>

#include "global.h"

DEFN_CREATABLE(PDFAttachmentView, AttachmentView);

PDFAttachmentView::PDFAttachmentView(QWidget *parent) :
    IndexedFileAttachmentView(parent),
    ui(new Ui::PDFAttachmentView)
{
    m_pdfWidget = new QWidget(this);
    ui->setupUi(m_pdfWidget);
    setWidget( m_pdfWidget );
}

PDFAttachmentView::~PDFAttachmentView()
{
    delete ui;
}

void PDFAttachmentView::polish()
{
    IndexedFileAttachmentView::polish();
}

QImage PDFAttachmentView::renderPDFDocument( Poppler::Document* doc )
{
    QList<QImage> pages;
    for (int i = 0; i < doc->numPages(); ++i)
    {
        pages << doc->page(i)->renderToImage( m_zoom * physicalDpiX(),
                                              m_zoom * physicalDpiY() );
    }

    assert(!pages.isEmpty());

    // assemble image
    int height = 0;
    int width = 0;
    QImage::Format format = pages.first().format();
    for (const QImage & i : pages)
    {
        height += i.height();
        width = qMax( width, i.width() );
        assert(format == i.format());
    }
    QImage allPages( width, height, format );
    QPainter painter( &allPages );
    for (int i = 0; i < pages.length(); ++i)
    {
        painter.drawImage( 0, 0, pages[i] );
        painter.translate( 0, pages[i].height() );
    }

    return allPages;
}

void PDFAttachmentView::open()
{
    Poppler::Document* doc = attachment<PDFAttachment>()->document();
    if (doc)
    {
        ui->label->setPixmap( QPixmap::fromImage( renderPDFDocument( doc ) ) );
    }
    else
    {
        ui->label->setPixmap( QPixmap() );
    }
}
