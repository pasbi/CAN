#include "attachmentview.h"
#include "Attachments/attachment.h"

AttachmentView::AttachmentView(QWidget *parent) :
    QWidget(parent),
    m_attachment( nullptr )
{
}

void AttachmentView::setAttachment(Attachment *attachment)
{
    reConnect(m_attachment, attachment, SIGNAL(destroyed()), this, SLOT(listenToAttachmentDestruction()));

    m_attachment = attachment;
    polish();
}

void AttachmentView::listenToAttachmentDestruction()
{
    m_attachment = nullptr;
}
