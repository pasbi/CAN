#include "chordpattern.h"
#include <QStringList>
#include "global.h"

const QString SPLIT_PATTERN = (QStringList() << QRegExp::escape("|") << QRegExp::escape(",")
                                             << QRegExp::escape("-") << QRegExp::escape("/")
                                             << "\\s"   ).join("|");

ChordPattern::ChordPattern(const QString &pattern)
{
    parse( pattern );
}

void ChordPattern::transpose(int t)
{
    for (Chord* chord : chords())
    {
        chord->transpose( t );
    }
}

bool isLineChordLine( const QString & line )
{
    QStringList tokens = line.split( QRegExp(SPLIT_PATTERN) );

    int numChords = 0;
    for (const QString & token : tokens)
    {
        if (Chord(token).isValid())
        {
            numChords++;
        }
    }
    return ((float) numChords * numChords * numChords / tokens.size()) >= 0.5;
}

QList<QPair<int, QString> > tokenize( QString line )
{
    QRegExp splitPattern = QRegExp(SPLIT_PATTERN);

    QList<QPair<int, QString>> tokens;

    int i = 0;
    int j = -1;
    while (( i = splitPattern.indexIn( line, j+1 ) ) >= 0)
    {
        QString token = line.mid( j+1, i - j - splitPattern.matchedLength() );
        if (!token.isEmpty())
        {
            tokens << qMakePair( j+1, token );
        }
        j = i;
    }
    QString token = line.mid(j+1, i - j - splitPattern.matchedLength());
    if (!token.isEmpty())
    {
        tokens << qMakePair( j+1, token );
    }

    return tokens;
}

QList<Chord> ChordPattern::parseChordLine(const QString &line)
{
    QList<QPair<int, QString>> tokens = tokenize( line );

    QList<Chord> chords;
    for ( const QPair<int, QString> & token : tokens )
    {
        Chord chord(token.second, token.first);
        if (chord.isValid())
        {
            chords.append( chord );
        }
    }

    return chords;
}

void ChordPattern::parse(const QString &text)
{
    QStringList lines = text.split("\n");

    for ( const QString & line : lines )
    {
        if (isLineChordLine(line))
        {
            m_lines << Line( parseChordLine( line ) );
        }
        else
        {
            m_lines << Line( line );
        }
    }
}

QList<const Chord*> ChordPattern::chords() const
{
    QList<const Chord*> cs;
    for (const Line & line : m_lines )
    {
        if (line.type() == Line::Chords)
        {
            cs << line.chords();
        }
    }
    return cs;
}

QList<Chord*> ChordPattern::chords()
{
    QList<Chord*> cs;
    for (Line & line : m_lines )
    {
        if (line.type() == Line::Chords)
        {
            cs << line.chords();
        }
    }
    return cs;
}

QString convert( QList<const Chord*> chords, Chord::MinorPolicy mpolicy, Chord::EnharmonicPolicy epolicy )
{
    QString text;

    int i = -1;
    for (const Chord * c : chords)
    {
        assert( c->column() > i );
        text.insert( c->column(), c->toString( mpolicy, epolicy ) );
    }
    return text;
}

QString ChordPattern::toString(Chord::MinorPolicy mpolicy, Chord::EnharmonicPolicy epolicy) const
{
    QStringList lines;
    for (const Line & line : m_lines)
    {
        if (line.type() == Line::Text)
        {
            lines << line.text();
        }
        else
        {
            lines << convert( line.chords(), mpolicy, epolicy );
        }
    }

    return lines.join("\n");
}


QList<const Chord*> Line::chords() const
{
    if (m_type == Chords)
    {
        QList<const Chord*> cs;
        for (const Chord & c : m_chords)
        {
            cs << &c;
        }
        return cs;
    }
    else
    {
        return QList<const Chord*>();
    }
}

QList<Chord*> Line::chords()
{
    if (m_type == Chords)
    {
        QList<Chord*> cs;
        for (Chord & c : m_chords)
        {
            cs << &c;
        }
        return cs;
    }
    else
    {
        return QList<Chord*>();
    }
}

























