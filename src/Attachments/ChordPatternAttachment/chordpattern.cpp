#include "chordpattern.h"
#include <QStringList>
#include "global.h"
#include <QJsonObject>
#include <QJsonArray>
#include "persistentobject.h"

const QString SPLIT_PATTERN = (QStringList() << QRegExp::escape("|") << QRegExp::escape(",")
                                             << QRegExp::escape("-") << QRegExp::escape("/")
                                             << QRegExp::escape("`") << "\\s"   ).join("|");

ChordPattern::ChordPattern(const QString &pattern )
{
    parse( pattern );
}

QList<const Chord*> Line::chords() const
{
    QList<const Chord*> cs;
    for (const Chord& c : m_chords)
    {
        cs << &c;
    }
    return cs;
}

void ChordPattern::transpose(int t)
{
    m_transpose += t;
    m_transpose %= 12;
    m_transpose += 12;
    m_transpose %= 12;
}

int ChordPattern::countChords(const QString &text)
{
    int count = 0;
    for (const QString & line : text.split("\n"))
    {
        for (const QString & token : line.split(QRegExp(SPLIT_PATTERN)))
        {
            if (Chord(token).isValid())
            {
                count++;
            }
        }
    }
    return count;
}

bool ChordPattern::isLineChordLine( const QString & line )
{
    QStringList tokens = line.split( QRegExp(SPLIT_PATTERN), QString::SkipEmptyParts );

    int numChords = countChords( line );

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

QList<Chord> ChordPattern::parseChordLine(const QString &line, int transpose )
{
    QList<QPair<int, QString>> tokens = tokenize( line );

    QList<Chord> chords;
    for ( const QPair<int, QString> & token : tokens )
    {
        Chord chord(token.second, transpose, token.first);
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
            m_lines << Line( parseChordLine( line, m_transpose ) );
        }
        else
        {
            m_lines << Line( line );
        }
    }
}

QString convert( QList<const Chord*> chords, Chord::MinorPolicy mpolicy, Chord::EnharmonicPolicy epolicy, int transpose )
{
    QString text;

    int i = -1;
    for (const Chord * c : chords)
    {
        assert( c->column() > i );
        qDebug() << "insert " << c << " at " << c->column() << "in" << text;
        text.insert( c->column(), c->toString( transpose, mpolicy, epolicy ) );
        qDebug() << text;
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
            lines << convert( line.chords(), mpolicy, epolicy, m_transpose );
        }
    }
    return lines.join("\n");
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

int Line::length( int transpose, Chord::MinorPolicy minorPolicy, Chord::EnharmonicPolicy enharmonicPolicy ) const
{
    switch (m_type)
    {
    case Chords:
    {
        int max = 0;
        for (const Chord& c : m_chords)
        {
            int current = c.toString( transpose, minorPolicy, enharmonicPolicy ).length() + c.column();
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

QJsonObject Line::toJsonObject() const
{
    QJsonObject object;
    switch (type())
    {
    case Chords:
    {
        object.insert("type", QString("chords"));
        QJsonArray chords;
        for (const Chord& c : m_chords)
        {
            chords.append(c.toJsonObject());
        }
        object.insert("chords", chords);
        break;
    }
    case Text:
        object.insert("type", QString("text"));
        object.insert("text", m_text);
        break;
    }

    return object;
}

Line Line::fromJsonObject(const QJsonObject &object)
{
    if (checkJsonObject( object, "type", QJsonValue::String ))
    {
        QString type = object["type"].toString();
        if (type == "text")
        {
            if ( checkJsonObject( object, "text", QJsonValue::String ) )
            {
                return Line(object["text"].toString());
            }
        }
        else if (type == "chords")
        {
            if ( checkJsonObject( object, "chords", QJsonValue::Array ) )
            {
                QList<Chord> chords;
                for (const QJsonValue & val : object["chords"].toArray() )
                {
                    if (val.type() == QJsonValue::Object)
                    {
                        chords << Chord(val.toObject());
                    }
                    else
                    {
                        WARNING << "Expected type Object.";
                    }
                }
                return Line( chords );
            }
        }
        else
        {
            WARNING << "Unknown type " << type;
        }
    }
    return Line("");
}

void ChordPattern::appendLine(const Line &line)
{
    m_lines.append( line );
}

void ChordPattern::clear()
{
    m_lines.clear();
}

















