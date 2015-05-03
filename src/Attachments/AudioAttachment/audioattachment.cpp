#include "audioattachment.h"

DEFN_CREATABLE_NAME(AudioAttachment, Attachment, "Audio Attachment")

AudioAttachment::AudioAttachment() :
    m_sectionsModel( new SectionsModel(this ) )
{
    setName( tr("Audio Attachment") );
    connect(this, SIGNAL(hashChanged(QByteArray)), this, SLOT(open()));
}

void AudioAttachment::open()
{
    m_player.open( filename() );
}

void AudioAttachment::copy(Attachment *&copied) const
{
    copied = new AudioAttachment();
    IndexedFileAttachment::copy( copied );
}
