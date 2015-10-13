#include "filefilter.h"
#include <QFileInfo>
#include <algorithm>

#include "util.h"
#include "global.h"

FileFilter::FileFilter()
{
}

QString normalizeString( QString s )
{

    const QStringList replaceBySpace = QStringList( {".", ":", ";", "-", "#",
                                                     "~", "*", "+", "_", ",",
                                                     "`", "´", "=", "/", "\\",
                                                     "!", "\"", "§", "$", "%", "@" });
    for (const QString & token : replaceBySpace)
    {
        s.replace(token, "");
    }

    // remove whitespaces
    s.replace(QRegExp("\\s+"), "");

    return Util::replaceDiacritics(s).toLower();
}

double computeScore(QString filename, const QStringList& normalizedKeys)
{
    qreal score = 0;


    filename = normalizeString(filename);
    QStringList tokens = filename.split("/", QString::SkipEmptyParts);
    int i = tokens.length();
    for (const QString& key : normalizedKeys)
    {
        if (filename.contains(key)) // this little if saves a hughe amount of time...
        {
            for (const QString& token : tokens)
            {
                QString nToken = token;
                if (nToken.contains(key))
                {
                    score += (qreal) key.length() / i;
                }
            }
            --i;
        }
    }
    return -score;
}

QStringList FileFilter::apply(const QStringList & candidateFilenames, const QStringList& keys, const QStringList& acceptedEndings) const
{
    QStringList normalizedKeys;
    for (const QString& key : keys)
    {
        normalizedKeys << normalizeString(key);
    }

    QMultiMap<qreal, QString> sortHelper;
    for (const QString& filename : candidateFilenames)
    {
        if (acceptedEndings.contains(QFileInfo(filename).suffix()))
        {
            double score = computeScore(filename, normalizedKeys);
            if (score != 0)
            {
                sortHelper.insert( score, filename );
            }
        }
    }
    return sortHelper.values().mid(0, 20);
}
