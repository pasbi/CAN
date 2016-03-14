#ifndef CHORDPATTERNATTACHMENT_H
#define CHORDPATTERNATTACHMENT_H

#include "abstractchordpatternattachment.h"
#include "chord.h"


class ChordPatternAttachment : public AbstractChordPatternAttachment
{
    Q_OBJECT
    DECL_CREATABLE(ChordPatternAttachment)

public:
    ChordPatternAttachment();

    QString chordPattern() const;

    void setPattern(const QString & pattern);

    void copy(Attachment *&copied) const;

    double scrollDownTempo() const;
    void setScrollDownTempo( double tempo );
    static QString process(QString text, int transpose = 0);

public slots:
    void transpose( int transposing = 0 );
};

#endif // CHORDPATTERNATTACHMENT_H
