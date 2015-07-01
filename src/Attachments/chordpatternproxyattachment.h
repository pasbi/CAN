#ifndef CHORDPATTERNPROXYATTACHMENT_H
#define CHORDPATTERNPROXYATTACHMENT_H

#include "../Attachments/ChordPatternAttachment/abstractchordpatternattachment.h"

class ChordPatternAttachment;
class ChordPatternProxyAttachment : public AbstractChordPatternAttachment
{
    Q_OBJECT
    DECL_CREATABLE(ChordPatternProxyAttachment)

public:
    ChordPatternProxyAttachment();

    QJsonObject toJsonObject() const;
    bool restoreFromJsonObject(const QJsonObject &object);

    void updateCache();
    void transpose( int t );
    int transpose() const { return m_transpose; }
    double scrollDownTempo() const;
    void setScrollDownTempo(double);

    QString chordPattern() const { return m_patternCache; }
    void copy(Attachment *&copied) const;

    const ChordPatternAttachment* source() const { return m_source; }

private:
    const ChordPatternAttachment* m_source;
    int m_transpose = 4;
    QString m_patternCache;
    void setChordPatternAttachment(const ChordPatternAttachment *source );

};

#endif // CHORDPATTERNPROXYATTACHMENT_H
