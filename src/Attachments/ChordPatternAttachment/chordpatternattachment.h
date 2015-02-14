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
    ChordPattern* chordPattern() { return &m_pattern; }
    void transpose(int t);


private:
    ChordPattern m_pattern;

signals:
    void changed();

};

#endif // CHORDPATTERNATTACHMENT_H
