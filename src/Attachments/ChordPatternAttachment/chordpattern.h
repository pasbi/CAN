#ifndef CHORDPATTERN_H
#define CHORDPATTERN_H

#include "chord.h"
#include <QPair>
#include <QList>
#include <QStringList>

#define private public

class Line
{
public:
    enum Type { Text, Chords };
    Line( const QString text ) :
        m_type( Text ),
        m_text( text ),
        m_chords( QList<Chord>() )
    {
    }

    Line( const QList<Chord> &  chords) :
        m_type( Chords ),
        m_text( "" ),
        m_chords( chords )
    {
    }

    QString text() const
    {
        return m_type == Text ? m_text : "";
    }

    QList<const Chord*> chords() const;

    Type type() const { return m_type; }
    int length(int transpose, Chord::MinorPolicy minorPolicy = Chord::LowerCase, Chord::EnharmonicPolicy enharmonicPolicy = Chord::Natural) const;
    QJsonObject toJsonObject() const;
    static Line fromJsonObject( const QJsonObject & object );

private:
    const Type m_type;
    QString m_text;
    QList<Chord> m_chords;

};

class ChordPattern
{
public:
    ChordPattern(const QString & pattern );

    QString toString(Chord::MinorPolicy mpolicy, Chord::EnharmonicPolicy epolicy = Chord::Natural ) const;

    void transpose( int t );
    int transpose() const { return m_transpose; }
    QList<const Line*> lines() const;

    static int countChords(const QString & text );
    static bool isLineChordLine( const QString & line );

    void clear();
    void appendLine( const Line & line );


private:
    void parse(const QString & text );
    QList<Chord> parseChordLine(const QString &line, int transpose);
    QList<Line> m_lines;
    int m_transpose = 0;
};

#endif // CHORDPATTERN_H
