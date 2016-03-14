#include "audioattachment.h"
#include "sectionsmodel.h"

DEFN_CREATABLE_NAME(AudioAttachment, Attachment, QT_TRANSLATE_NOOP("Creatable", "Audio Attachment"))

AudioAttachment::AudioAttachment() :
    m_sectionsModel( new SectionsModel(this ) )
{
    addAttributeKey("sections");
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

void AudioAttachment::serialize(QDataStream &out) const
{
    IndexedFileAttachment::serialize(out);
    out << m_sectionsModel;
    out << static_cast<qint32>(sectionsModel()->indexOf(m_player.currentSection()));
}

void AudioAttachment::deserialize(QDataStream &in)
{
    IndexedFileAttachment::deserialize(in);
    qint32 index;
    in >> m_sectionsModel;
    in >> index;
    if (index < 0)
    {
        m_player.setCurrentSection(Section());
    }
    else
    {
        m_player.setCurrentSection( sectionsModel()->sections()[index] );
    }
}

QStringList AudioAttachment::skipSerializeAttributes() const
{
    QStringList skipped = IndexedFileAttachment::skipSerializeAttributes();
    skipped << "sections";
    return skipped;
}

QVariant AudioAttachment::attribute(const QString &key) const
{
    if (key == "sections")
    {
        return QVariant::fromValue(m_sectionsModel->sections());
    }
    else
    {
        return IndexedFileAttachment::attribute(key);
    }
}

void AudioAttachment::setAttribute(const QString &key, const QVariant &value)
{
    if (key == "sections")
    {
        // use sectionsModel() to add/remove/change sections.
        Q_UNREACHABLE();
    }
    else
    {
        IndexedFileAttachment::setAttribute(key, value);
    }
}
