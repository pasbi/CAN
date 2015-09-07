#include "filefilter.h"
#include <QFileInfo>
#include <algorithm>

#include "util.h"

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

double computeScore(const QString& filename, const QStringList& normalizedKeys)
{
    qreal score = 0;

    QStringList tokens = filename.split("/", QString::SkipEmptyParts);
    int i = tokens.length();
    for (const QString& token : tokens)
    {
        for (const QString& key : normalizedKeys)
        {
            QString nToken = normalizeString(token);
            if (nToken.contains(key))
            {
                score += (qreal) key.length() / i;
            }
        }
        --i;
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












// obsolete

//double rank( const QString & candidate, const QString& title, const QString& artist, const QStringList& endings )
//{
//    QFileInfo fileInfo( candidate );
//    if (!endings.contains(fileInfo.suffix()))
//    {
//        return 0;
//    }

//    QString candidateWithoutEnding = QString("%1/%2").arg(fileInfo.path(), fileInfo.baseName());

//    QStringList levels = candidateWithoutEnding.split("/", QString::SkipEmptyParts);
//    QList<QStringList> levelss;
//    int i = 0;
//    for (const QString& level : levels)
//    {
//        levelss << normalizeString(level).split(" ", QString::SkipEmptyParts);
//        i++;
//    }
//    QStringList attributes = QStringList() << title << artist;

//    double rank = 0;
//    for (int i = 0; i < levelss.length(); ++i)
//    {
//        double factor = qPow(0.7, i);
//        double count = 0;
//        for (const QString& token : levelss[i])
//        {
//            if (attributes.contains( token ))
//            {
//                if (token.length() == 1)
//                {
//                    count += 0.1;
//                }
//                else if (token.length() == 2)
//                {
//                    count += 0.2;
//                }
//                else if (token.length() == 3)
//                {
//                    count += 0.8;
//                }
//                else
//                {
//                    count += 1;
//                }
//            }
//        }
//        rank += factor * count;
//    }
//    return rank;
//}

//QStringList filter( const QStringList &             candidates,
//                    const QString&                  title,
//                    const QString&                  artist,
//                    const QStringList &             endings )
//{
//    QList<QPair<QString, double>> ranking;
//    for ( const QString & candidate : candidates )
//    {
//        double score = rank(candidate, title, artist, endings);
//        if (score > 0)
//        {
//            ranking.append( qMakePair(candidate, score) );
//        }
//    }

//    qSort( ranking.begin(), ranking.end(), rankingGreaterThan );

//    QStringList result;
//    for (const QPair<QString, double> & p : ranking)
//    {
//        result << p.first;
//    }

//    return result;
//}
