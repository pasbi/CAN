#include "pdfattachment.h"

DEFN_CREATABLE(PDFAttachment, Attachment);

PDFAttachment::PDFAttachment()
{
    setName( tr("PDF Attachment") );
}

void PDFAttachment::copy(Attachment*& attachment) const
{

}


void PDFAttachment::open()
{
    delete m_document;
    m_document = Poppler::Document::load( filename() );
    qDebug() << "open pdf doc: " << filename();
    assert(m_document);
    m_document->setRenderHint(Poppler::Document::TextAntialiasing);
}
