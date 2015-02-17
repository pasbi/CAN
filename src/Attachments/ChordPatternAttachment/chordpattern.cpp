#include "chordpattern.h"
#include <QStringList>
#include "global.h"
#include <QJsonObject>
#include <QJsonArray>
#include "persistentobject.h"
#if 0

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







#endif









