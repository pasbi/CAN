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

    QString chordPattern() const { return m_pattern; }

    QJsonObject toJsonObject() const;
    bool restoreFromJsonObject(const QJsonObject &object);

    void setPattern(const QString & pattern);

    void copy(Attachment *&copied) const;

    double scrollDownTempo() const { return m_scrollDownTempo; }
    void setScrollDownTempo( double tempo );
    static QString process(QString text, int transpose = 0);

public slots:
    void transpose( int transposing = 0 );


private:
    QString m_pattern;

    double m_scrollDownTempo;

protected:
    void serialize(QDataStream &out) const;
    void deserialize(QDataStream &in);
};

#endif // CHORDPATTERNATTACHMENT_H
