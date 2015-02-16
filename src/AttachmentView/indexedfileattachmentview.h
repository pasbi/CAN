#ifndef INDEXEDFILEATTACHMENTVIEW_H
#define INDEXEDFILEATTACHMENTVIEW_H

#include "attachmentview.h"

namespace Ui {
class IndexedFileAttachmentView;
}

class IndexedFileAttachmentView : public AttachmentView
{
    Q_OBJECT

public:
    explicit IndexedFileAttachmentView(QWidget *parent = 0);
    ~IndexedFileAttachmentView();

    void setWidget(QWidget *widget);

private:
    Ui::IndexedFileAttachmentView *ui;
};

#endif // INDEXEDFILEATTACHMENTVIEW_H
