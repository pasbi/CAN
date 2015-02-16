#ifndef AUDIOATTACHMENT_H
#define AUDIOATTACHMENT_H

#include "Attachments/indexedfileattachment.h"

class AudioAttachment : public IndexedFileAttachment
{
    Q_OBJECT
    DECL_CREATABLE( AudioAttachment );
public:
    AudioAttachment();
    void copy(Attachment *&attachment) const;
};

#endif // AUDIOATTACHMENT_H
