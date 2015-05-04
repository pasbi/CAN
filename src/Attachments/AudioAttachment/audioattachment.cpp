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
    if (m_filename != filename())
    {
        m_filename = filename();
        m_player.open( m_filename );
    }
}

void AudioAttachment::copy(Attachment *&copied) const
{
    copied = new AudioAttachment();
    IndexedFileAttachment::copy( copied );
}

void AudioAttachment::appendSection(const Section &section)
{
    sectionsModel()->appendSection( section );
    setSection( sectionsModel()->section( sectionsModel()->rowCount() - 1 ) );
}

void AudioAttachment::setSection(const Section *section)
{
    m_currentSection = section;
    player().setSection( m_currentSection );
    emit currentSectionChanged( m_currentSection );
}

QJsonObject AudioAttachment::toJsonObject() const
{
    QJsonObject object = IndexedFileAttachment::toJsonObject();

    QJsonArray sections = sectionsModel()->toJson();

    object["sections"] = sections;
    object["currentSection"] = sectionsModel()->indexOf(m_currentSection);
    return object;

}

bool AudioAttachment::restoreFromJsonObject(const QJsonObject &object)
{
    IndexedFileAttachment::restoreFromJsonObject( object );
    sectionsModel()->restore( object["sections"].toArray() );
    int i = object["currentSection"].toDouble();
    if (i >= 0)
    {
        setSection( sectionsModel()->section(i) );
    }

    return     checkJsonObject( object, "sections", QJsonValue::Array )
            && checkJsonObject( object, "currentSection", QJsonValue::Double);
}
