#ifndef PDFATTACHMENT_H
#define PDFATTACHMENT_H

#include "indexedfileattachment.h"

class PDFAttachment : public Attachment
{
    Q_OBJECT
    DECL_CREATABLE(PDFAttachment);
public:
    PDFAttachment();
    void copy(Attachment*& attachment) const;


};

#endif // PDFATTACHMENT_H
