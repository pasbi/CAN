#include "chordpatternattachment.h"

#include "attachment.h"

DEFN_CREATABLE(ChordPatternAttachment, Attachment);

ChordPatternAttachment::ChordPatternAttachment()
{
    setName( tr("Chord Pattern") );
}
