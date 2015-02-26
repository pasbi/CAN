#include "attachmentview.h"

AttachmentView::AttachmentView(QWidget *parent) :
    QWidget(parent),
    m_attachment( NULL )
{
}

void AttachmentView::setAttachment(Attachment *attachment)
{
    m_attachment = attachment;
    polish();
}
