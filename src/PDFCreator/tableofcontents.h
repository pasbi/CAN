#ifndef TABLEOFCONTENTS_H
#define TABLEOFCONTENTS_H

#include <QString>
#include <QList>
#include "global.h"

class EnumerationNumber
{
public:
    EnumerationNumber() { m_numbers = QList< int >() << 1; }
    EnumerationNumber( const EnumerationNumber& num ) { m_numbers = num.m_numbers; }

    int level() const { return m_numbers.length(); }
    int operator[] (int i) { assert(i >= 0 && i < level()); return m_numbers[i]; }

    EnumerationNumber increase() const;
    EnumerationNumber down() const;
    EnumerationNumber up( int targetLevel ) const;

    QString toString(QString pattern) const;

private:
    QList< int > m_numbers; // saves e.g. 1.3.4.2

    enum NumberType { Arabic, RomanCapital, Roman, AlphabetCapital, Alphabet };
    static QString toNumber( const int number, const NumberType type );
};

class TocEntry
{
public:
    TocEntry( const QString & label, const int level, const EnumerationNumber & number, const int pageNumber) :
        label( label ),
        level( level ),
        number( number ),
        pageNumber( pageNumber )
    { }

    const QString label;
    const int level;
    const EnumerationNumber number;
    const int pageNumber;

};

class TableOfContents
{
public:
    TableOfContents() {}
    void append( const QString & label, const int level, const int pageNumber )
    {
        bool success = true;
        if ( m_entries.isEmpty() )
        {
            if (level != 1)
            {
                qWarning() << "cannot insert item at level" << level;
                success = false;
            }
            else
            {
                // nothing to do!
            }
        }
        else if (m_entries.last().level == level)
        {
            // insert at same level
            m_lastNumber.increase();
        }
        else if (m_entries.last().level== level + 1)
        {
            // level down
            m_lastNumber.down();
        }
        else if (m_entries.last().level > level)
        {
            // up
            m_lastNumber.up( level );
            m_lastNumber.increase();
        }
        else
        {
            qWarning() << "cannot insert item at level" << level << "(preceeding level: " << m_entries.last().level << ")";
            success = false;
        }

        if (success)
        {
            m_entries.append( TocEntry( label, level, m_lastNumber, pageNumber ));
        }
    }

//    void appendSame( const QString & label )
//    {
//        int level = 0;
//        if (!m_entries.isEmpty())
//        {
//            level = m_entries.last().level;
//        }
//        append( label, level );
//    }

//    void appendBelow( const QString & label )
//    {
//        int level = 0;
//        if (!m_entries.isEmpty())
//        {
//            level = m_entries.last().level + 1;
//        }
//        append( label, level );
//    }

    QList< TocEntry > entries() const { return m_entries; }

    bool isEmpty() const { return m_entries.isEmpty(); }
    TocEntry take() { return m_entries.takeFirst(); }

private:
    QList< TocEntry > m_entries;
    EnumerationNumber m_lastNumber;
};


#endif // TABLEOFCONTENTS_H
