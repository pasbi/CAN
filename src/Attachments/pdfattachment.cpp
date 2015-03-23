#include "pdfattachment.h"
#include "application.h"

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
    assert(m_document);
    m_document->setRenderHint(Poppler::Document::TextAntialiasing);
}
