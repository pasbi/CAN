#include "audioattachment.h"

DEFN_CREATABLE_NAME(AudioAttachment, Attachment, "Audio Attachment")

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
