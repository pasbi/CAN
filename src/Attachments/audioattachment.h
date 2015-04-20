#ifndef AUDIOATTACHMENT_H
#define AUDIOATTACHMENT_H

#include "Attachments/indexedfileattachment.h"

class AudioAttachment : public IndexedFileAttachment
{
    Q_OBJECT
    DECL_CREATABLE( AudioAttachment )
public:
    AudioAttachment();
    QStringList acceptedEndings() const { return QStringList() << "mp3" << "ogg"; }
    void copy(Attachment *&copied) const;


public slots:
    void open();

};

#endif // AUDIOATTACHMENT_H
