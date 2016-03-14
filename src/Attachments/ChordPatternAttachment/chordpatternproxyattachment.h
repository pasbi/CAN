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
    int transpose() const;
    double scrollDownTempo() const;
    void setScrollDownTempo(double);

    QString chordPattern() const { return m_patternCache; }
    void copy(Attachment *&copied) const;

    ChordPatternAttachment* source() const;

private:
    void setChordPatternAttachment(ChordPatternAttachment *source );
    QString m_patternCache;

    QStringList skipSerializeAttributes() const;

private slots:
    void adjustSourceTransposing(int sourceTransposing);

protected:
    void serialize(QDataStream &out) const;
    void deserialize(QDataStream &in);
};

#endif // CHORDPATTERNPROXYATTACHMENT_H
