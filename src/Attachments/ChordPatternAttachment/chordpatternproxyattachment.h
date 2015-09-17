#ifndef CHORDPATTERNPROXYATTACHMENT_H
#define CHORDPATTERNPROXYATTACHMENT_H

#include "abstractchordpatternattachment.h"

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

protected:
    void serialize(QDataStream &out) const;
    void deserialize(QDataStream &in);

private:
    void setChordPatternAttachment(const ChordPatternAttachment *source );
    const ChordPatternAttachment* m_source;
    int m_transpose = 4;
    QString m_patternCache;

private slots:
    void adjustSourceTransposing(int sourceTransposing);

};

#endif // CHORDPATTERNPROXYATTACHMENT_H
