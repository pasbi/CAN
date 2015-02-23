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

public slots:
    void updateAttachmentView();

private:
    AttachmentView* m_currentView = 0;
    QScrollArea* m_scrollArea = 0;
    QMap<Attachment*, QByteArray> m_attachmentViewOptions;

};

#endif // ATTACHMENTEDITOR_H
