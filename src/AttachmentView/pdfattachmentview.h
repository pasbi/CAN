#ifndef PDFATTACHMENTVIEW_H
#define PDFATTACHMENTVIEW_H

#include "indexedfileattachmentview.h"
#include "attachmentview.h"

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
    void connectWithAttachment();

private:
    Ui::PDFAttachmentView *ui;
    QWidget* m_pdfWidget = NULL;
};

#endif // PDFATTACHMENTVIEW_H
