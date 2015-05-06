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

protected slots:
    virtual void open() = 0;
private:
    Ui::IndexedFileAttachmentView *ui;
private slots:
    void updateStackedWidget();

    void chooseFile();

private://tmp
    static QList<Ui::IndexedFileAttachmentView*> initializedUis;
    static QList<Ui::IndexedFileAttachmentView*> deletedUis;
};

#endif // INDEXEDFILEATTACHMENTVIEW_H
