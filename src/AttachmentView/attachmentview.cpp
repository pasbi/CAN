#include "attachmentview.h"

AttachmentView::AttachmentView(QWidget *parent) :
    QWidget(parent)
{
}

void AttachmentView::setAttachment(Attachment *attachment)
{
    qDebug() << this << metaObject()->className();
    m_attachment = attachment;
    connectWithAttachment();
}
