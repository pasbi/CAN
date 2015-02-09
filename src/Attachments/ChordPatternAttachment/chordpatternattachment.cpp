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
