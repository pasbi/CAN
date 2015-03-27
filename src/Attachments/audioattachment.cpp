#include "audioattachment.h"

DEFN_CREATABLE( AudioAttachment, Attachment );

AudioAttachment::AudioAttachment()
{
    setName( tr("Audio Attachment") );
}

void AudioAttachment::open()
{

}

void AudioAttachment::copy(Attachment *&copied) const
{
    copied = new AudioAttachment();
    IndexedFileAttachment::copy( copied );
}
