#include "attachmentview.h"
#include "Attachments/attachment.h"

AttachmentView::AttachmentView(QWidget *parent) :
    QWidget(parent),
    m_attachment( nullptr )
{
}

void AttachmentView::setAttachment(Attachment *attachment)
{
    if (m_attachment)
    {
        disconnect(m_attachment, SIGNAL(destroyed()), this, SLOT(listenToAttachmentDestruction()));
    }

    m_attachment = attachment;
    polish();

    if (m_attachment)
    {
        connect(m_attachment, SIGNAL(destroyed()), this, SLOT(listenToAttachmentDestruction()));
    }
}

void AttachmentView::listenToAttachmentDestruction()
{
    m_attachment = nullptr;
}
