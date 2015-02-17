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
    const ChordPattern* chordPattern() const { return &m_pattern; }
    void transpose(int t);

    void copy(Attachment *&attachment) const;

    QJsonObject toJsonObject() const;
    bool restoreFromJsonObject(const QJsonObject &object);

private:
    ChordPattern m_pattern;

signals:
    void changed();

};

#endif // CHORDPATTERNATTACHMENT_H
