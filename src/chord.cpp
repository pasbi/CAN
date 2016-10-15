#include "chord.h"
#include "global.h"
#include <QDataStream>

const QString Chord::SPLIT_PATTERN = (QStringList() << QRegExp::escape("|") << QRegExp::escape(",")
                                             << QRegExp::escape("-") << QRegExp::escape("/")
                                             << QRegExp::escape("`") << QRegExp::escape("*") << "\\s"   ).join("|");

const QString Chord::IGNORE_BEFORE_PATTERN = "(" + (QStringList() << QRegExp::escape("(") << QRegExp::escape("[")
                                               << QRegExp::escape("{")  << QRegExp::escape("<")).join("|") + ")*";

const QString Chord::IGNORE_AFTER_PATTERN = "(" + (QStringList() << QRegExp::escape(")") << QRegExp::escape("]")
                                               << QRegExp::escape("}")  << QRegExp::escape(">") << QRegExp::escape("?")
                                               << QRegExp::escape(".") << "\\(\\w*\\)" << "\\[\\w*\\]").join("|") + ")*";


const QString CHORD_EXTENSION_PATTERN = "(maj|min|2|4|5|7th|maj7|min7|sus4|sus2|sus|°|dim|dim7|aug|6|min6|"
                                        "9|min9|maj9|11|min11|maj11|13|min13|maj13|add9|maj7th|7|b5|\\+|_)*$" ;

const QString Chord::FLAT = QChar(0x266D);
const QString Chord::SHARP = QChar(0x266F);

Chord::Chord(const QString token) :
    m_isValid( parse(token) )
{
}

Chord::Chord() :
    m_isValid(false)
{

}

QString Chord::baseString() const
{
    Chord::EnharmonicPolicy epolicy = Chord::Natural; //static_cast<Chord::EnharmonicPolicy>(config["EnharmonicPolicy"].toInt());

    switch (m_base)
    {
    case 0:
        return "C";
    case 1:
        switch (epolicy)
        {
        case Sharp:
            return "C" + SHARP;
        case Natural:
        case Flat:
            return "D" + FLAT;
        }
    case 2:
        return "D";
    case 3:
        switch (epolicy)
        {
        case Sharp:
            return "D" + SHARP;
        case Natural:
        case Flat:
            return "E" + FLAT;
        }
    case 4:
        return "E";
    case 5:
        return "F";
    case 6:
        switch (epolicy)
        {
        case Sharp:
        case Natural:
            return "F" + SHARP;
        case Flat:
            return "G" + FLAT;
        }
    case 7:
        return "G";
    case 8:
        switch (epolicy)
        {
        case Sharp:
            return "G" + SHARP;
        case Flat:
        case Natural:
            return "A" + FLAT;
        }
    case 9:
        return "A";
    case 10:
        switch (epolicy)
        {
        case Sharp:
            return "A" + SHARP;
        case Flat:
        case Natural:
            return "B" + FLAT;
        }
    case 11:
        return "B";
    default:
        return "?";
    }
}

QString Chord::toString() const
{
    return m_before + key() + m_attachment + m_after;
}


bool Chord::parseLine( const QString & line, QStringList & chords, QStringList & tokens )
{
    tokens = line.split(QRegExp(SPLIT_PATTERN));

    int numWords = 0;
    for (const QString & token : tokens)
    {
        if (Chord(token).isValid())
        {
            chords << token;
        }
        else if (!token.isEmpty() && !token.contains(QRegExp(QString("[^A-Za-z%1']").arg(Util::diacriticLetters()))))
        {
            numWords++;
        }
    }
    return chords.length() > numWords;
}

int parseBase( const QChar & c )
{
    switch (c.toUpper().cell())
    {
    case 'C':
        return 0;
    case 'D':
        return 2;
    case 'E':
        return 4;
    case 'F':
        return 5;
    case 'G':
        return 7;
    case 'A':
        return 9;
    case 'B':
        return 11;
    default:
        return -1;
    }
}

bool ifStartsWithTake( QString & string, const QString & pattern )
{
    if (string.startsWith(pattern))
    {
        string = string.mid( pattern.length() );
        return true;
    }
    return false;
}

bool Chord::parse(QString text)
{
    if (text.isEmpty())
    {
        return false;
    }

    QRegExp beforeMatcher( "^(" + IGNORE_BEFORE_PATTERN + ")" );
    if (beforeMatcher.indexIn( text ) == 0 )
    {
        int n = beforeMatcher.matchedLength();
        m_before = text.left(n);
        text = text.mid(n);
    }

    QRegExp afterMatcher( "(" + IGNORE_AFTER_PATTERN + ")$" );
    if ( afterMatcher.indexIn( text ) >= 0 )
    {
        int n = afterMatcher.matchedLength();
        m_after = text.right( n );
        text = text.mid( 0, text.length() - n );
    }

    QChar baseChar = text[0];
    m_base = parseBase(baseChar);
    if (m_base < 0)
    {
        return false;
    }

    text = text.mid(1);

    if ( ifStartsWithTake( text, "es" )
     || (!text.startsWith("sus") && ifStartsWithTake( text, "s") )
     ||  ifStartsWithTake( text, "b" )
     ||  ifStartsWithTake( text, FLAT)    )
    {
        m_base--;
    }

    if ( ifStartsWithTake( text, "is" )
     ||  ifStartsWithTake( text, "#" )
     ||  ifStartsWithTake( text, SHARP ) )
    {
        m_base++;
    }

    m_base %= 12;
    m_base += 12;
    m_base %= 12;

    if ( baseChar.isLower()
         || (!text.startsWith("min") && !text.startsWith("maj") && ifStartsWithTake( text, "m" ) ) )
    {
        m_isMinor = true;
    }
    else
    {
        m_isMinor = false;
    }

    m_attachment = text;
    if (QRegExp( CHORD_EXTENSION_PATTERN ).exactMatch( m_attachment ))
    {
        return true;
    }
    else
    {
        return false;
    }
}

void Chord::transpose(int t)
{
    m_base += t;
    m_base %= 12;
    m_base += 12;
    m_base %= 12;
}

QString Chord::key() const
{
    if (isValid())
    {

        Chord::MinorPolicy mpolicy = static_cast<Chord::MinorPolicy>(preference("MinorPolicy").toInt());
        QString base = baseString();
        if (isMinor())
        {
            switch (mpolicy)
            {
            case LowerCase:
                return base.toLower();
                break;
            case FollowingM:
                return base + "m";
                break;
            default:
                Q_UNREACHABLE();
                return "";
            }
        }
        else
        {
            return base;
        }
    }
    else
    {
        return "";
    }
}

bool Chord::operator ==(const Chord& other) const
{
    if (!m_isValid && !other.m_isValid)
    {
        return true;
    }
    else
    {
        return m_isValid == other.m_isValid
        && m_before == other.m_before
        && m_after == other.m_after
        && m_base == other.m_base
        && m_attachment == other.m_attachment
        && m_isMinor == other.m_isMinor;
    }
}


QDataStream& operator<<(QDataStream& out, const Chord& chord)
{
    qint32 base = chord.m_base;
    qint32 isMinor = static_cast<qint32>(chord.m_isMinor);
    qint32 isValid = static_cast<qint32>(chord.m_isValid);
    out << chord.m_after << chord.m_attachment << base << chord.m_before << isMinor << isValid;
    return out;
}

QDataStream& operator>>(QDataStream& in, Chord& chord)
{
    qint32 base;
    qint32 isMinor;
    qint32 isValid;
    in >> chord.m_after >> chord.m_attachment >> base >> chord.m_before >> isMinor >> isValid;
    chord.m_base = static_cast<int>(base);
    chord.m_isMinor = static_cast<bool>(isMinor);
    chord.m_isValid = static_cast<bool>(isValid);
    return in;
}

QDebug& operator <<(QDebug& dbg, const Chord& chord)
{
    dbg << chord.toString();
    return dbg;
}























