#ifndef INDEXEDFILEATTACHMENTVIEW_H
#define INDEXEDFILEATTACHMENTVIEW_H

#include "AttachmentView/attachmentview.h"

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

protected slots:
    virtual void polish();
    virtual void open();
    virtual void chooseFile();

private:
    Ui::IndexedFileAttachmentView *ui;

private slots:
    void updateStackedWidget();

};

#endif // INDEXEDFILEATTACHMENTVIEW_H
