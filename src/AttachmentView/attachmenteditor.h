#ifndef ATTACHMENTEDITOR_H
#define ATTACHMENTEDITOR_H

#include <QWidget>
#include "Attachments/attachment.h"
#include "attachmentview.h"
#include <QScrollArea>

class AttachmentEditor : public QWidget
{
    Q_OBJECT
public:
    explicit AttachmentEditor(QWidget *parent = 0);
    void setAttachment(Attachment* attachment);

private:
    AttachmentView* m_attachmentView = 0;
    QScrollArea* m_scrollArea = 0;

};

#endif // ATTACHMENTEDITOR_H
