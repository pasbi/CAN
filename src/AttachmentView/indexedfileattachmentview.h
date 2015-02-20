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
    virtual void polish();

    void updateAttachmentView();

signals:
    void fileSelected( QByteArray hash );

private:
    Ui::IndexedFileAttachmentView *ui;
};

#endif // INDEXEDFILEATTACHMENTVIEW_H
