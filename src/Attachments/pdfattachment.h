#ifndef PDFATTACHMENT_H
#define PDFATTACHMENT_H

#include "indexedfileattachment.h"
#include "poppler.h"


class PDFAttachment : public IndexedFileAttachment
{
    Q_OBJECT
    DECL_CREATABLE(PDFAttachment)
public:
    PDFAttachment();
    QStringList acceptedEndings() const { return QStringList() << "pdf"; }
#ifdef HAVE_POPPLER
    Poppler::Document* document() const { return m_document; }
#endif

    void copy(Attachment *&copied) const;

public slots:
    void open();


private:
#ifdef HAVE_POPPLER
    Poppler::Document* m_document = NULL;
#endif



};

#endif // PDFATTACHMENT_H
