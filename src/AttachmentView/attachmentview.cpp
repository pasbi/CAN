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

template<typename T>
T* AttachmentView::attachment() const
{
    T* attachment = qobject_cast<T*>( m_attachment );
    assert( attachment );
    return attachment;
}
