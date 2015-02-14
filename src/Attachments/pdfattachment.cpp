#include "pdfattachment.h"

DEFN_CREATABLE(PDFAttachment, Attachment);

PDFAttachment::PDFAttachment()
{
    setName( tr("PDF Attachment") );
}


void PDFAttachment::copy(Attachment *&attachment) const
{
    attachment = new PDFAttachment();
    IndexedFileAttachment::copy( attachment );

}
