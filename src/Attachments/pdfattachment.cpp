#include "pdfattachment.h"

DEFN_CREATABLE(PDFAttachment, Attachment);

PDFAttachment::PDFAttachment()
{
    setName( tr("PDF Attachment") );
}
