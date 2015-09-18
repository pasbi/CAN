#include "chordpatternproxyattachment.h"
#include "chordpatternattachment.h"
#include "Database/SongDatabase/song.h"
#include "application.h"

DEFN_CREATABLE_NAME(ChordPatternProxyAttachment, Attachment, QT_TRANSLATE_NOOP("Creatable", "Chord Proxy Attachment"))

ChordPatternProxyAttachment::ChordPatternProxyAttachment() :
    AbstractChordPatternAttachment(),
    m_source( NULL )
{
    setName( tr("Proxy Pattern") );

    const Attachment* a = app().currentAttachment();
    if (a && a->inherits( "ChordPatternAttachment" ))
    {
        setChordPatternAttachment( qobject_assert_cast<const ChordPatternAttachment*>( a ) );
    }
    else
    {
        setChordPatternAttachment( NULL );
    }
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
    assert(!m_source);
    if (source)
    {
        m_source = source;
        updateCache();
        emit changed();
        connect(m_source, SIGNAL(transposed(int)), this, SLOT(adjustSourceTransposing(int)));
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

void ChordPatternProxyAttachment::serialize(QDataStream &out) const
{
    AbstractChordPatternAttachment::serialize(out);
    out << static_cast<qint32>(song()->attachments().indexOf(const_cast<ChordPatternAttachment*>(m_source)));
    out << static_cast<qint32>(m_transpose);
}

void ChordPatternProxyAttachment::deserialize(QDataStream &in)
{
    AbstractChordPatternAttachment::deserialize(in);
    qint32 index, transpose;
    in >> index >> transpose;
    if (index < 0)
    {
        m_source = nullptr;
    }
    else
    {
        // we can assume that the source is deserialized first, so it is already existent in song()->attachments()
        setChordPatternAttachment( qobject_assert_cast<ChordPatternAttachment*>(song()->attachments()[index]) );
        assert(m_source);
    }
    m_transpose = transpose;
    updateCache();
}

void ChordPatternProxyAttachment::adjustSourceTransposing(int sourceTransposing)
{
    transpose( -sourceTransposing );
}

