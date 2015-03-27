#include "pdfattachment.h"
#include "application.h"

DEFN_CREATABLE_NAME(PDFAttachment, Attachment, "PDF Attachment");

PDFAttachment::PDFAttachment()
{
    setName( tr("PDF Attachment") );
}

void PDFAttachment::open()
{
    delete m_document;
    m_document = Poppler::Document::load( filename() );
    qDebug() << filename() << hash();
    assert(m_document);
    m_document->setRenderHint(Poppler::Document::TextAntialiasing);
}

void PDFAttachment::copy(Attachment *&copied) const
{
    copied = new PDFAttachment();
    IndexedFileAttachment::copy( copied );
}
