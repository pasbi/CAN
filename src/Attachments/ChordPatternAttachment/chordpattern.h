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
    QList<Chord*> chords();

    Type type() const { return m_type; }

    int length(Chord::MinorPolicy minorPolicy = Chord::LowerCase, Chord::EnharmonicPolicy enharmonicPolicy = Chord::Natural) const;

private:
    const Type m_type;
    QString m_text;
    QList<Chord> m_chords;


};

class ChordPattern
{
public:
    ChordPattern( const QString & pattern );

    QString toString(Chord::MinorPolicy mpolicy, Chord::EnharmonicPolicy epolicy = Chord::Natural ) const;

    void transpose( int t );
    QList<const Chord*> chords() const;
    QList<Chord*> chords();
    QList<const Line*> lines() const;

private:
    void parse( const QString & text );
    QList<Chord> parseChordLine(const QString &line);
    QList<Line> m_lines;
};

#endif // CHORDPATTERN_H
