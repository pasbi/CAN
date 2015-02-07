#ifndef CHORD_H
#define CHORD_H

#include <QString>

class Chord
{
public:

    enum EnharmonicPolicy { Sharp, Flat, Natural };
    enum MinorPolicy { LowerCase, FollowingM };

    Chord( const QString token, int m_column = -1 );

    void transpose( int t );
    void resetTranspose();
    int transpose() const { return m_transpose; }
    int base() const { return m_base; }
    QString baseString(EnharmonicPolicy epolicy) const;
    QString attachment() const { return m_attachment; }
    QString toString(MinorPolicy mpolicy, EnharmonicPolicy epolicy = Natural) const;
    bool isValid() const { return m_isValid; }
    int column() const { return m_column; }
    bool isMinor() const { return m_isMinor; }

private:
    int m_base;
    int m_transpose;
    QString m_attachment;
    bool parse(QString text);
    static QString flat(const QString& s = "");
    static QString sharp(const QString& s = "");

    const bool m_isValid;
    const int m_column = -1;
    bool m_isMinor;
};

#endif // CHORD_H
