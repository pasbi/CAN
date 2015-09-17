#include "audioattachment.h"
#include "sectionsmodel.h"

DEFN_CREATABLE_NAME(AudioAttachment, Attachment, QT_TRANSLATE_NOOP("Creatable", "Audio Attachment"))

AudioAttachment::AudioAttachment() :
    m_sectionsModel( new SectionsModel(this ) )
{
    setName( tr("Audio Attachment") );
    connect(this, SIGNAL(hashChanged(QByteArray)), this, SLOT(open()));
}

void AudioAttachment::open()
{
    if (m_filename != filename())
    {
        m_filename = filename();
        m_player.open( m_filename );
    }
}

void AudioAttachment::appendSection(const Section &section)
{
    sectionsModel()->appendSection( section );
    setSection( sectionsModel()->section( sectionsModel()->rowCount() - 1 ) );
}

void AudioAttachment::setSection(const Section *section)
{
    m_currentSection = section;
    emit currentSectionChanged( m_currentSection );
}

void AudioAttachment::serialize(QDataStream &out) const
{
    IndexedFileAttachment::serialize(out);
    out << m_sectionsModel;
    out << static_cast<qint32>(sectionsModel()->indexOf(m_currentSection));
}

void AudioAttachment::deserialize(QDataStream &in)
{
    IndexedFileAttachment::deserialize(in);
    qint32 index;
    in >> m_sectionsModel;
    in >> index;
    setSection( sectionsModel()->section(index) );
}
