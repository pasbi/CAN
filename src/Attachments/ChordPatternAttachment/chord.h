#ifndef CHORD_H
#define CHORD_H

#include <QString>
#include <QJsonObject>

class Chord
{
public:

    enum EnharmonicPolicy { Sharp, Flat, Natural };
    enum MinorPolicy { LowerCase, FollowingM };

    Chord( const QString token, int transpose = 0, int m_column = -1 );
    Chord( const QJsonObject & object );

    void resetTranspose();
    int base() const { return m_base; }
    QString baseString(EnharmonicPolicy epolicy, int tranpose) const;
    QString attachment() const { return m_attachment; }
    QString toString(int tranpose, MinorPolicy mpolicy, EnharmonicPolicy epolicy = Natural) const;
    bool isValid() const { return m_isValid; }
    int column() const { return m_column; }
    bool isMinor() const { return m_isMinor; }

    QJsonObject toJsonObject() const;

private:
    int m_base;
    QString m_attachment;
    bool parse(QString text, int transpose);
    static QString flat(const QString& s = "");
    static QString sharp(const QString& s = "");

    const bool m_isValid;
    int m_column = -1;
    bool m_isMinor;
};

#endif // CHORD_H
