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
public slots:
    virtual void chooseFile();

private:
    Ui::IndexedFileAttachmentView *ui;

    // this widget holds the actual content of the view (e.g. player/pdf/...)
    // you may use sth. like ui->setup(contentWidget()) in inherited class constructor
private:
    QWidget* m_contentWidget = nullptr;
protected:
    QWidget* contentWidget() const { return m_contentWidget; }

private slots:
    void updateStackedWidget();

};

#endif // INDEXEDFILEATTACHMENTVIEW_H
