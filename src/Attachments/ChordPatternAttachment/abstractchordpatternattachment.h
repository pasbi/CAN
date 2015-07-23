#ifndef ABSTRACTCHORDPATTERNATTACHMENT_H
#define ABSTRACTCHORDPATTERNATTACHMENT_H

#include "../attachment.h"

class AbstractChordPatternAttachment : public Attachment
{
    Q_OBJECT
public:
    AbstractChordPatternAttachment();

    virtual QString chordPattern() const = 0;
    virtual void transpose( int i ) = 0;
    virtual double scrollDownTempo() const = 0;
    virtual void setScrollDownTempo(double tempo) = 0;

signals:
    void changed();
};

#endif // ABSTRACTCHORDPATTERNATTACHMENT_H
