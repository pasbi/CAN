#include "audioattachmentview.h"

DEFN_CREATABLE( AudioAttachmentView, AttachmentView );

AudioAttachmentView::AudioAttachmentView(QWidget* parent) :
    IndexedFileAttachmentView(parent)
{
}

void AudioAttachmentView::polish()
{
    IndexedFileAttachmentView::polish();
}
