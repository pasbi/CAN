#include "chordpatternproxyattachment.h"
#include "chordpatternattachment.h"
#include "Database/SongDatabase/song.h"
#include "application.h"

DEFN_CREATABLE_NAME(ChordPatternProxyAttachment, Attachment, QT_TRANSLATE_NOOP("Creatable", "Chord Proxy Attachment"))

ChordPatternProxyAttachment::ChordPatternProxyAttachment() :
    AbstractChordPatternAttachment()
{
    setName( tr("Proxy Pattern") );

    addAttributeKey("sourceChordPatternAttachment");
    addAttributeKey("transpose");
    setAttribute("sourceChordPatternAttachment", QVariant::fromValue<void*>(nullptr));

    Attachment* a = app().currentAttachment();
    if (a && a->inherits( "ChordPatternAttachment" ))
    {
        setChordPatternAttachment( qobject_assert_cast<ChordPatternAttachment*>( a ) );
    }
    else
    {
        setChordPatternAttachment( nullptr );
    }
}

void ChordPatternProxyAttachment::transpose(int t)
{
    t += attribute("transpose").toInt();
    t %= 12;
    t += 12;
    t %= 12;
    setAttribute("transpose", t);
    updateCache();
}

int ChordPatternProxyAttachment::transpose() const
{
    return attribute("transpose").toInt();
}

void ChordPatternProxyAttachment::setChordPatternAttachment( ChordPatternAttachment *newSource )
{
    Q_ASSERT(!source());
    if (newSource)
    {
        setAttribute("sourceChordPatternAttachment", QVariant::fromValue<void*>(static_cast<void*>(newSource)));
        updateCache();
        emit changed();
        connect(source(), SIGNAL(transposed(int)), this, SLOT(adjustSourceTransposing(int)));
    }
}

void ChordPatternProxyAttachment::updateCache()
{
    if (source())
    {
        QString pattern = ChordPatternAttachment::process( source()->chordPattern(), transpose() );
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

ChordPatternAttachment* ChordPatternProxyAttachment::source() const
{
    return static_cast<ChordPatternAttachment*>(attribute("sourceChordPatternAttachment").value<void*>());
}

double ChordPatternProxyAttachment::scrollDownTempo() const
{
    if (source())
    {
        return source()->scrollDownTempo();
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

void ChordPatternProxyAttachment::adjustSourceTransposing(int sourceTransposing)
{
    transpose( -sourceTransposing );
}


QStringList ChordPatternProxyAttachment::skipSerializeAttributes() const
{
    QStringList skip = AbstractChordPatternAttachment::skipSerializeAttributes();
    skip << "sourceChordPatternAttachment";
    return skip;
}


void ChordPatternProxyAttachment::serialize(QDataStream& out) const
{
    AbstractChordPatternAttachment::serialize(out);
    out << static_cast<qint32>(song()->attachments().indexOf(const_cast<ChordPatternAttachment*>(source())));
    out << static_cast<qint32>(transpose());
}

void ChordPatternProxyAttachment::deserialize(QDataStream& in)
{
    AbstractChordPatternAttachment::deserialize(in);
    qint32 index, transpose;
    in >> index >> transpose;

    if (index < 0)
    {
        setAttribute("sourceChordPatternAttachment", QVariant::fromValue<void*>(nullptr));
    }
    else
    {
        // we can assume that the source is deserialized first, so it is already existent in song()->attachments()
        setChordPatternAttachment( qobject_assert_cast<ChordPatternAttachment*>(song()->attachments()[index]) );
        Q_ASSERT(source());
    }
    setAttribute("transpose", transpose);
    updateCache();
}

