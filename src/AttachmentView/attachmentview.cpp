#include "attachmentview.h"

AttachmentView::AttachmentView(QWidget *parent) :
    QWidget(parent)
{
}

void AttachmentView::setAttachment(Attachment *attachment)
{
    m_attachment = attachment;
    connectWithAttachment();
}
