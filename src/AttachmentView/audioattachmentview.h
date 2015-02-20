#ifndef AUDIOATTACHMENTVIEW_H
#define AUDIOATTACHMENTVIEW_H

#include "indexedfileattachmentview.h"

class AudioAttachmentView : public IndexedFileAttachmentView
{
    Q_OBJECT
    DECL_CREATABLE( AudioAttachmentView );
public:
    AudioAttachmentView(QWidget* parent = 0);

protected:
    void polish();
};

#endif // AUDIOATTACHMENTVIEW_H
