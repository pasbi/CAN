#include "chord.h"
#include "global.h"
#include "persistentobject.h"

// *?
const QString Chord::SPLIT_PATTERN = (QStringList() << QRegExp::escape("|") << QRegExp::escape(",")
                                             << QRegExp::escape("-") << QRegExp::escape("/")
                                             << QRegExp::escape("`") << QRegExp::escape("*") << "\\s"   ).join("|");

const QString Chord::IGNORE_BEFORE_PATTERN = "(" + (QStringList() << QRegExp::escape("(") << QRegExp::escape("[")
                                               << QRegExp::escape("{")  << QRegExp::escape("<")).join("|") + ")*";

const QString Chord::IGNORE_AFTER_PATTERN = "(" + (QStringList() << QRegExp::escape(")") << QRegExp::escape("]")
                                               << QRegExp::escape("}")  << QRegExp::escape(">")
                                               << QRegExp::escape(".") << "\\(\\w*\\)" << "\\[\\w*\\]").join("|") + ")*";


const QString CHORD_EXTENSION_PATTERN = "(maj|min|5|7th|maj7|min7|sus4|sus2|sus|°|dim|dim7|aug|6|min6|"
                                        "9|min9|maj9|11|min11|maj11|13|min13|maj13|add9|maj7th|7|\\+)*$" ;

Chord::Chord(const QString token) :
    m_isValid( parse(token) )
{
}

QString Chord::flat(const QString& s)
{
    return QString("%1%2").arg(s).arg(QChar(0x266D));
}

QString Chord::sharp(const QString& s)
{
    return QString("%1%2").arg(s).arg(QChar(0x266F));
}

QString Chord::baseString( EnharmonicPolicy epolicy ) const
{
    switch (m_base)
    {
    case 0:
        return "C";
    case 1:
        switch (epolicy)
        {
        case Sharp:
            return sharp("C");
        case Natural:
        case Flat:
            return flat("D");
        }
    case 2:
        return "D";
    case 3:
        switch (epolicy)
        {
        case Sharp:
            return sharp("D");
        case Natural:
        case Flat:
            return flat("E");
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
            return sharp("F");
        case Flat:
            return flat("G");
        }
    case 7:
        return "G";
    case 8:
        switch (epolicy)
        {
        case Sharp:
            return sharp("G");
        case Flat:
        case Natural:
            return flat("A");
        }
    case 9:
        return "A";
    case 10:
        switch (epolicy)
        {
        case Sharp:
            return sharp("A");
        case Flat:
        case Natural:
            return flat("B");
        }
    case 11:
        return "B";
    default:
        return "?";
    }
}

QString Chord::toString(MinorPolicy mpolicy, EnharmonicPolicy epolicy) const
{
    QString text = baseString(epolicy);

    if (isMinor())
    {
        switch (mpolicy)
        {
        case LowerCase:
            text = text.toLower();
            break;
        case FollowingM:
            text += "m";
            break;
        }
    }

    return m_before + text + attachment() + m_after;
}


bool Chord::parseLine( const QString & line, QStringList & chords, QStringList & tokens )
{
    tokens = line.split(QRegExp(SPLIT_PATTERN));

    int numberOfUncertainChords = 0;

    int numToken = 0;
    for (const QString & token : tokens)
    {
        if (Chord(token).isValid())
        {
            chords << token;
            if ( token == "a" )
            {
                numberOfUncertainChords++;
            }
        }
        // do only count tokens that contains letters or numbers etc.
        if (token.contains(QRegExp("[A-Za-z0-9]")))
        {
            numToken++;
        }
    }

    const int numChords = chords.length();
    return (double) (numChords - numberOfUncertainChords) / qMax(0, (numToken - numChords)) > 0.8;
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
     ||  ifStartsWithTake( text, flat())    )
    {
        m_base--;
    }

    if ( ifStartsWithTake( text, "is" )
     ||  ifStartsWithTake( text, "#" )
     ||  ifStartsWithTake( text, sharp() ) )
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































