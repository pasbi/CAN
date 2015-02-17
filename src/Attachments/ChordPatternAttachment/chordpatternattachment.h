#ifndef CHORDPATTERNATTACHMENT_H
#define CHORDPATTERNATTACHMENT_H

#include "../attachment.h"
#include "chord.h"


class ChordPatternAttachment : public Attachment
{
    Q_OBJECT
    DECL_CREATABLE(ChordPatternAttachment);
public:
    ChordPatternAttachment();
    QString chordPattern() const { return m_pattern; }

    void copy(Attachment *&attachment) const;

    QJsonObject toJsonObject() const;
    bool restoreFromJsonObject(const QJsonObject &object);

    void setPattern( const QString & pattern );
    void process(int transpose = 0);


private:
    QString m_pattern;
    Chord::EnharmonicPolicy m_enharmonicPolicy = Chord::Natural;
    Chord::MinorPolicy      m_minorPolicy      = Chord::LowerCase;

signals:
    void changed();

};

#endif // CHORDPATTERNATTACHMENT_H
