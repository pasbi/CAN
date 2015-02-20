#include "audioattachment.h"

DEFN_CREATABLE( AudioAttachment, Attachment );

AudioAttachment::AudioAttachment()
{
    setName( tr("Audio Attachment") );
}

void AudioAttachment::copy(Attachment *&attachment) const
{

}

void AudioAttachment::loadContent()
{

}
