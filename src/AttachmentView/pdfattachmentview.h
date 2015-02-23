#ifndef PDFATTACHMENTVIEW_H
#define PDFATTACHMENTVIEW_H

#include "indexedfileattachmentview.h"
#include "attachmentview.h"
#include <poppler/qt5/poppler-qt5.h>


namespace Ui {
class PDFAttachmentView;
}

class PDFAttachmentView : public IndexedFileAttachmentView
{
    Q_OBJECT
    DECL_CREATABLE(PDFAttachmentView);

public:
    explicit PDFAttachmentView(QWidget *parent = 0);
    ~PDFAttachmentView();

protected:
    void polish();

protected slots:
    void open();

private:
    Ui::PDFAttachmentView *ui;
    QWidget* m_pdfWidget = NULL;
    double m_zoom = 1;
    QImage renderPDFDocument( Poppler::Document* doc );
};

#endif // PDFATTACHMENTVIEW_H
