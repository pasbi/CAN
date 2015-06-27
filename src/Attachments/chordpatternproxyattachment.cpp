#include "chordpatternproxyattachment.h"
#include "chordpatternattachment.h"
#include "Database/SongDatabase/song.h"

DEFN_CREATABLE_NAME(ChordPatternProxyAttachment, Attachment, QT_TRANSLATE_NOOP("Creatable", "Chord Proxy Attachment"))

ChordPatternProxyAttachment::ChordPatternProxyAttachment() :
    m_source( NULL )
{
    setName( tr("Proxy Pattern") );
}

void ChordPatternProxyAttachment::copy(Attachment *&copied) const
{
    copied = new ChordPatternProxyAttachment();
    ChordPatternProxyAttachment* cpa = dynamic_cast<ChordPatternProxyAttachment*>(copied);
    cpa->m_source = m_source;
    cpa->m_transpose = m_transpose;
    cpa->updateCache();
}

void ChordPatternProxyAttachment::transpose(int t)
{
    m_transpose += t;
    m_transpose %= 12;
    m_transpose += 12;
    m_transpose %= 12;
    updateCache();
}

void ChordPatternProxyAttachment::setChordPatternAttachment( const ChordPatternAttachment *source )
{
    if (m_source != source)
    {
        m_source = source;
        updateCache();
        emit changed();
    }
}

void ChordPatternProxyAttachment::updateCache()
{
    if (m_source)
    {
        QString pattern = ChordPatternAttachment::process( m_source->chordPattern(), m_transpose );
        if (pattern != m_patternCache)
        {
            m_patternCache = pattern;
            emit changed();
        }
    }
    else
    {
        m_patternCache = "";
    }
}

double ChordPatternProxyAttachment::scrollDownTempo() const
{
    if (m_source)
    {
        return m_source->scrollDownTempo();
    }
    else
    {
        return 0;
    }
}

void ChordPatternProxyAttachment::setScrollDownTempo(double)
{
    qWarning() << "cannot set scroll down tempo of ChordPatternProxyAttachment as it would change the scroll down tempo of the original.";
}

QJsonObject ChordPatternProxyAttachment::toJsonObject() const
{
    QJsonObject object = Attachment::toJsonObject();

    int i = -1;
    for (const Attachment* a : song()->attachments())
    {
         if (a == m_source)
         {
             ++i;
             break;
         }
         else
         {
             ++i;
         }
    }

    object.insert("source", i );
    object.insert("transpose", m_transpose);

    return object;
}

bool ChordPatternProxyAttachment::restoreFromJsonObject(const QJsonObject &object)
{
    bool success = true;
    if (checkJsonObject( object, "transpose", QJsonValue::Double ))
    {
        m_transpose = object["transpose"].toInt();
    }
    else
    {
        m_transpose = 0;
        success = false;
    }
    if (checkJsonObject( object, "source", QJsonValue::Double ))
    {
        int i = object["source"].toInt();
        if (i < 0)
        {
            m_source = NULL;
        }
        else if (i >= song()->attachments().length())
        {
            qWarning() << "source does not exist.";
            m_source = NULL;
        }
        else
        {
            Attachment* a = song()->attachment(i);
            if (a->inherits("ChordPatternAttachment"))
            {
                m_source = qobject_assert_cast<ChordPatternAttachment*>( a );
                updateCache();
            }
            else
            {
                m_source = NULL;
                qWarning() << "wrong source type";
            }
        }
    }

    return Attachment::restoreFromJsonObject( object );
}
