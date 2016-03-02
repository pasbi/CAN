#ifndef CHORD_H
#define CHORD_H

#include <QString>
#include <QVariant>
#include "global.h"

class Chord
{
public:

    enum EnharmonicPolicy { Sharp, Natural, Flat };
    enum MinorPolicy { LowerCase, FollowingM };

    Chord( const QString token );
    Chord();
    Chord(const Chord& other) = default;

    /**
     * @brief base returns the base (C, D, E, ...) of the chord
     * @return the base (C, D, E, ...) of the chord
     */
    QString baseString() const;

    /**
     * @brief toString returns the full representation of the chord
     * @return the string representation of the full chord (e.g. Cm7(repeat))
     */
    QString toString() const;

    /**
     * @brief key returns the key of this chord (e.g. Cm, a, F, but never C7, or Am(repeat)).
     * @return the key of this chord.
     */
    QString key() const;

    /**
     * @brief isValid returns true if this is a valid chord
     * @return if this is a valid chord
     */
    bool isValid() const { return m_isValid; }

    /**
     * @brief isMinor returns if this is a minor chord
     * @return if this is a minor chord
     */
    bool isMinor() const { return m_isMinor; }

    /**
     * @brief transpose transpose the chord by t half tones
     * @param t the number of half tones
     */
    void transpose(int t);


    /**
     * @brief countChords returns the number of chords in @code line.
     *  Assumes @code line has no linebreaks. E.g. "C\nD" is treated as it is: a string of length 3 with an unkown char in the middle.
     * @param line
     * @return false if an error occured, false otherwise
     */
    static bool parseLine(const QString &line, QStringList &chords, QStringList &tokens);

    bool operator ==(const Chord& other) const;
    bool operator !=(const Chord& other) const { return !(*this == other); }
    DEFINE_NONSENSE_SMALLER_OPERATOR(Chord)



private:
    QString m_before, m_after;
    int m_base;
    QString m_attachment;
    bool m_isValid;
    bool m_isMinor;

    /**
     * @brief parse parses the chord and fills the fields.
     * @param text the parsed text.
     * @return if the parsing was successfull.
     */
    bool parse(QString text);

public:
    // nice version of #
    static const QString FLAT;
    // nice version of b
    static const QString SHARP;
    static const QString SPLIT_PATTERN;
    static const QString IGNORE_BEFORE_PATTERN;
    static const QString IGNORE_AFTER_PATTERN;


    friend QDataStream& operator<<(QDataStream& out, const Chord& chord);
    friend QDataStream& operator>>(QDataStream& in, Chord& chord);
};

QDataStream& operator<<(QDataStream& out, const Chord& chord);
QDataStream& operator>>(QDataStream& in, Chord& chord);
QDebug& operator<<(QDebug& dbg, const Chord& chord);

Q_DECLARE_METATYPE(Chord)
REGISTER_META_TYPE_STREAM_OPERATORS(Chord, Chord)
REGISTER_META_TYPE_COMPARE_OPERATORS(Chord, Chord)

#endif // CHORD_H
