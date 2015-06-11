#ifndef CHORDPATTERNATTACHMENT_H
#define CHORDPATTERNATTACHMENT_H

#include "../attachment.h"
#include "chord.h"


class ChordPatternAttachment : public Attachment
{
    Q_OBJECT
    DECL_CREATABLE(ChordPatternAttachment)
public:
    ChordPatternAttachment();
    ChordPatternAttachment(const Attachment & attachment);

    QString chordPattern() const { return m_pattern; }

    QJsonObject toJsonObject() const;
    bool restoreFromJsonObject(const QJsonObject &object);

    void setPattern(const QString & pattern);

    void copy(Attachment *&copied) const;

    double scrollDownTempo() const { return m_scrollDownTempo; }
    void setScrollDownTempo( double tempo ) { m_scrollDownTempo = tempo; }

public slots:
    void process( int transposing = 0 );


private:
    QString m_pattern;
    Chord::EnharmonicPolicy m_enharmonicPolicy = Chord::Natural;
    Chord::MinorPolicy      m_minorPolicy      = Chord::LowerCase;

    QString process(QString text, int transpose = 0);
    double m_scrollDownTempo;

signals:
    void changed();

};

#endif // CHORDPATTERNATTACHMENT_H
