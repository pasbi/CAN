#ifndef PDFATTACHMENT_H
#define PDFATTACHMENT_H

#include "indexedfileattachment.h"
#include "poppler/qt5/poppler-qt5.h"


class PDFAttachment : public IndexedFileAttachment
{
    Q_OBJECT
    DECL_CREATABLE(PDFAttachment);
public:
    PDFAttachment();
    void copy(Attachment*& attachment) const;
    QStringList acceptedEndings() const { return QStringList() << "pdf"; }
    Poppler::Document* document() const { return m_document; }

protected slots:
    void open();


private:
    Poppler::Document* m_document = NULL;



};

#endif // PDFATTACHMENT_H