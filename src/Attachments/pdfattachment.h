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

public slots:
    void open();

    bool isPaintable() const { return true; }
    void paint(PDFCreator *pdfCreator);


private:
#ifdef HAVE_POPPLER
    Poppler::Document* m_document = nullptr;
#endif

};

#endif // PDFATTACHMENT_H
