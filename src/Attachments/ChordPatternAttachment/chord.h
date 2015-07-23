#ifndef CHORD_H
#define CHORD_H

#include <QString>
#include <QJsonObject>

class Chord
{
public:

    enum EnharmonicPolicy { Sharp, Natural, Flat };
    enum MinorPolicy { LowerCase, FollowingM };

    Chord( const QString token );

    int base() const { return m_base; }
    QString baseString(EnharmonicPolicy epolicy) const;
    QString attachment() const { return m_attachment; }
    QString toString(MinorPolicy mpolicy, EnharmonicPolicy epolicy = Natural) const;
    bool isValid() const { return m_isValid; }
    bool isMinor() const { return m_isMinor; }
    void transpose(int t);

    static const QString SPLIT_PATTERN;
    static const QString IGNORE_BEFORE_PATTERN;
    static const QString IGNORE_AFTER_PATTERN;

    /**
     * @brief countChords returns the number of chords in @code line.
     *  Assumes @code line has no linebreaks. E.g. "C\nD" is treated as it is, a string of length 3 with an unkown char in the middle."
     * @param line
     * @return
     */
    static bool parseLine(const QString &line, QStringList &chords, QStringList &tokens);
private:
    QString m_before, m_after;
    int m_base;
    QString m_attachment;
    bool parse(QString text);
    static QString flat(const QString& s = "");
    static QString sharp(const QString& s = "");

    const bool m_isValid;
    bool m_isMinor;
};

#endif // CHORD_H
