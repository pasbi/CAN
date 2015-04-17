#include "tableofcontents.h"

// slightly modified from `http://rosettacode.org/wiki/Roman_numerals/Encode#C.2B.2B`
QString roman( int number, bool capital )
{
    struct romandata_t { int value; char const* numeral; };
    static romandata_t const romandata[] =
    {
        1000, "M",
        900, "CM",
        500, "D",
        400, "CD",
        100, "C",
        90, "XC",
        50, "L",
        40, "XL",
        10, "X",
        9, "IX",
        5, "V",
        4, "IV",
        1, "I",
        0, NULL }; // end marker

    QString result;
    for (romandata_t const* current = romandata; current->value > 0; ++current)
    {
        while (number >= current->value)
        {
            result.append( current->numeral );
            number  -= current->value;
        }
    }

    if (!capital)
    {
        return result.toLower();
    }
    else
    {
        return result;
    }
}

QString alpha( int number, bool capital )
{
    QString result;
    while (number != 0)
    {
        result.prepend(QChar::fromLatin1( 65 + (number % 26) ));
        number /= 26;
    }

    if (!capital)
    {
        return result.toLower();
    }
    else
    {
        return result;
    }
}

QString EnumerationNumber::toNumber(const int number, const NumberType type)
{
    switch (type)
    {
    case Arabic:
        return QString("%1").arg(number);
    case RomanCapital:
        return roman( number, true );
    case Roman:
        return roman( number, false );
    case AlphabetCapital:
        return alpha( number, true );
    case Alphabet:
        return alpha( number, false );
    default:
        return "";
    }
}

QString EnumerationNumber::toString(QString pattern) const
{
    static QMap< QChar, NumberType > numberTypeMap = QMap< QChar, NumberType >(
    { { '1', Arabic },
      { 'I', RomanCapital },
      { 'i', Roman },
      { 'A', AlphabetCapital },
      { 'a', Alphabet }         } );

    int count = 0;

    // find positions of numbers and truncate pattern to correct length,
    QList<QPair<int, NumberType>> tokens;
    for (int i = 0; i < pattern.length(); ++i)
    {
        if ( numberTypeMap.keys().contains( pattern.at(i) ) )
        {
            count++;
            tokens << qMakePair( i, numberTypeMap[pattern.at(i)] );
        }
        if ( count == level() )
        {
            pattern = pattern.left( i+1 );
            break;
        }
    }

    for (int i = tokens.length() - 1; i >= 0; --i)
    {
        QString s = toNumber( m_numbers[i], tokens[i].second );
        pattern.replace( tokens[i].first, 1, s );
    }

    return pattern;
}

EnumerationNumber EnumerationNumber::increase() const
{
    assert(!m_numbers.isEmpty());
    EnumerationNumber copy = *this;
    copy.m_numbers.last()++;
    return copy;
}

EnumerationNumber EnumerationNumber::down() const
{
    EnumerationNumber copy = *this;
    copy.m_numbers.append(1);
    return copy;
}

EnumerationNumber EnumerationNumber::up( int targetLevel ) const
{
    assert( targetLevel <= level() && targetLevel > 0 );
    EnumerationNumber copy = *this;
    while (copy.level() >= targetLevel)
    {
        copy.m_numbers.removeLast();
    }
    return copy;
}


