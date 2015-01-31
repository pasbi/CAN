#ifndef PDFATTACHMENTVIEW_H
#define PDFATTACHMENTVIEW_H

#include "attachmentview.h"
#include "creatable.h"

namespace Ui {
class PDFAttachmentView;
}

class PDFAttachmentView : public AttachmentView, public Creatable
{
    Q_OBJECT
    DECL_CREATABLE(PDFAttachmentView);

public:
    explicit PDFAttachmentView(QWidget *parent = 0);
    ~PDFAttachmentView();

protected:
    void connectWithAttachment() {}

private:
    Ui::PDFAttachmentView *ui;
};

#endif // PDFATTACHMENTVIEW_H
