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
    QStringList tokens = line.split( QRegExp(SPLIT_PATTERN), QString::SkipEmptyParts );

    int numChords = 0;
    for (const QString & token : tokens)
    {
        if (Chord(token).isValid())
        {
            numChords++;
        }
    }

    bool isChordLine = ((float) numChords * numChords / tokens.size()) >= 0.5;
    return isChordLine;
}

QList<QPair<int, QString> > tokenize( QString line )
{
    QRegExp splitPattern = QRegExp(SPLIT_PATTERN);

    QList<QPair<int, QString>> tokens;

    int i = 0;
    int j = -1;
    while (( i = splitPattern.indexIn( line, j+1 ) ) >= 0)
    {
        int n = splitPattern.matchedLength();
        QString token = line.mid( j+1, i - j - n );
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
    m_lines.clear();
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

QList<const Line*> ChordPattern::lines() const
{
    QList<const Line*> ls;
    for (int i = 0; i < m_lines.length(); ++i)
    {
        ls << &m_lines[i];
    }
    return ls;
}

int Line::length( Chord::MinorPolicy minorPolicy, Chord::EnharmonicPolicy enharmonicPolicy ) const
{
    switch (m_type)
    {
    case Chords:
    {
        int max = 0;
        for (const Chord& c : m_chords)
        {
            int current = c.toString( minorPolicy, enharmonicPolicy ).length() + c.column();
            max = qMax( current, max );
        }
        return max;
    }
    case Text:
        return m_text.length();
    default:
        assert(false);
        return 0;
    }
}






















