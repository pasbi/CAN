#include "file.h"
#include <QRegExp>
#include <QStringList>
#include "global.h"
#include <QStack>
#include <QFileInfo>

const QRegExp BEGIN_CONFLICT = QRegExp("^<<<<<<< .+$");
const QRegExp END_CONFLICT = QRegExp("^>>>>>>> .+$");
const QRegExp SEPARATE_CONFLICT = QRegExp("^=======$");

File::File( const QString & filename ) :
    m_content( readFile(filename).split("\n") ),
    m_filename( filename ),
    m_conflicts( findConflicts() )
{
}

QString File::readFile( const QString & filename )
{
    QFile file(filename);
    file.open(QIODevice::ReadOnly);
    return file.readAll();
}

QList<Conflict> File::findConflicts()
{
    QList<Conflict> conflicts;

    State state = NoConflict;

    QString currentType;
    QStringList localContentLines;
    QStringList remoteContentLines;

    int i = 0;


    for (const QString & line : m_content)
    {
        switch (state)
        {
        case NoConflict:
            if ( BEGIN_CONFLICT.exactMatch(line) )
            {
                qDebug() << "type of " << line << i;
                currentType = determineType( i );
                qDebug() << currentType;
                state = InLocal;
            }
            else if ( END_CONFLICT.exactMatch(line) )
            {
                WARNING << "detected end-of-conflict token without begin conflict";
            }
            else if ( SEPARATE_CONFLICT.exactMatch(line) )
            {
                WARNING << "detected conflict separator without begin conflict";
            }
            else
            {
                // no conflict, ignore that line
            }
            break;
        case InLocal:
            if ( BEGIN_CONFLICT.exactMatch(line) )
            {
                WARNING << "Begin conflict in conflict.";
                state = InLocal;
            }
            else if ( END_CONFLICT.exactMatch(line) )
            {
                WARNING << "detected end-of-conflict token without conflict separator";
            }
            else if ( SEPARATE_CONFLICT.exactMatch(line) )
            {
                state = InRemote;
            }
            else
            {
                localContentLines.append( line );
            }
            break;
        case InRemote:
            if ( BEGIN_CONFLICT.exactMatch(line) )
            {
                WARNING << "Begin conflict in conflict.";
                state = InLocal;
            }
            else if ( END_CONFLICT.exactMatch(line) )
            {
                state = NoConflict;
                conflicts << Conflict( currentType, localContentLines.join("\n"), remoteContentLines.join("\n") );
                localContentLines.clear();
                remoteContentLines.clear();
            }
            else if ( SEPARATE_CONFLICT.exactMatch(line) )
            {
                state = InRemote;
                WARNING << "detected conflict separator in remote part";
            }
            else
            {
                remoteContentLines.append( line );
            }
            break;
        }
        i++;
    }

    qDebug() << "conflicts = " << conflicts.size();
    return conflicts;
}

bool scopeOpens(const QString & line )
{
    return line.trimmed() == "{";
}

bool scopeCloses(const QString & line )
{
    return line.trimmed() == "}";
}

int getDepth( const QString & line )
{
    for (int i = 0; i < line.length(); ++i)
    {
        if (line[i] != ' ')
        {
            return i;
        }
    }

    return line.length();
}

bool isQuotation( const QString & line, const int pos )
{
    if (line[pos] == '\"')
    {
        if ( pos == 0 )
        {
            return true;
        }
        else
        {
            if (line[pos-1] == '\\')
            {
                return false;
            }
            else
            {
                return true;
            }
        }
    }
    else
    {
        return false;
    }
}

int positionOutsideQuotation(const QString & line, const QChar & character)
{
    bool insideQuotation = false;
    for (int i = 0; i < line.length(); ++i)
    {
        if ( isQuotation(line, i ))
        {
            insideQuotation = !insideQuotation;
        }
        else if ( line[i] == character )
        {
            return i;
        }
    }
    return -1;
}

QString key( const QString & line )
{
    int colonPos = positionOutsideQuotation( line, ':');

    if (colonPos < 0)
    {
        return QString();
    }
    else
    {
        QString key = line.left(colonPos).trimmed();
        key = key.mid(1, key.length() - 2);    // remove "s
        return key;
    }
}

bool insideList( const QStringList & lines, int lineNumber )
{
    for (int i = lineNumber; i >= 0; --i)
    {
        if ( positionOutsideQuotation( lines[i], '[' ) >= 0 )
        {
            return true;
        }
        if (scopeOpens( lines[i] ))
        {
            return false;
        }
    }
    return false;
}

QString File::determineType(int lineNumber)
{
    // we assume well-formated json, like Qt produces with specific settings.
    assert( lineNumber < m_content.length() );
    QStringList keys = QStringList() << QFileInfo(filename()).fileName();

    int currentDepth = getDepth( m_content[lineNumber] );

    bool searchKey = insideList( m_content, lineNumber ); // skip the current key, except we are in a list
    for (int i = lineNumber; i >= 0; --i)
    {
        QString line = m_content[i];
        if (searchKey)
        {
            qDebug() << ">>     key line: " << line;
            QString currentKey = key( line );
            if (!currentKey.isNull())
            {
                searchKey = false;
                keys << currentKey;
            }
        }
        else
        {
            if (scopeOpens( line ))
            {
                int depth = getDepth( line );
                if ( depth < currentDepth )
                {
                    currentDepth = depth;
                    searchKey = true;
                }
            }
        }
    }


    return keys.join("/");
}

