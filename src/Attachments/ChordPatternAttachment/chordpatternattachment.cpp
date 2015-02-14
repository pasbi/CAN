#include "chordpatternattachment.h"

#include "../attachment.h"

DEFN_CREATABLE(ChordPatternAttachment, Attachment);

ChordPatternAttachment::ChordPatternAttachment() :
    m_pattern( "" )
{
    setName( tr("Chord Pattern") );
}

QString ChordPatternAttachment::text(Chord::MinorPolicy minorPolicy, Chord::EnharmonicPolicy enharmonicPolicy) const
{
    return m_pattern.toString( minorPolicy, enharmonicPolicy );
}

void ChordPatternAttachment::transpose( int t )
{
    m_pattern.transpose( t );
    emit changed();
}

void ChordPatternAttachment::copy(Attachment *&attachment) const
{
    attachment = new ChordPatternAttachment();
    static_cast<ChordPatternAttachment*>(attachment)->m_pattern = m_pattern;
}
