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
    Poppler::Document* document() const { return m_document; }

    void copy(Attachment *&copied) const;

public slots:
    void open();


private:
    Poppler::Document* m_document = NULL;



};

#endif // PDFATTACHMENT_H
