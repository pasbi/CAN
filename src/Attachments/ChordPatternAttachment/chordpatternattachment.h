#ifndef CHORDPATTERNATTACHMENT_H
#define CHORDPATTERNATTACHMENT_H

#include "../attachment.h"
#include "chordpattern.h"

class ChordPatternAttachment : public Attachment
{
    Q_OBJECT
    DECL_CREATABLE(ChordPatternAttachment);
public:
    ChordPatternAttachment();
    QString text( Chord::MinorPolicy minorPolicy, Chord::EnharmonicPolicy enharmonicPolicy ) const;
    ChordPattern& chordPattern() { return m_pattern; }
    const ChordPattern& chordPattern() const { return m_pattern; }

private:
    ChordPattern m_pattern;

};

#endif // CHORDPATTERNATTACHMENT_H
