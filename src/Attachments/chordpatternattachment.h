#ifndef CHORDPATTERNATTACHMENT_H
#define CHORDPATTERNATTACHMENT_H

#include "attachment.h"

class ChordPatternAttachment : public Attachment
{
    DECL_CREATABLE(ChordPatternAttachment);
public:
    ChordPatternAttachment();
    QString bark() { return "CPA"; }

};

#endif // CHORDPATTERNATTACHMENT_H
