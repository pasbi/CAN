#ifndef FILEFILTER_H
#define FILEFILTER_H

#include <QStringList>

class FileFilter
{
public:
    FileFilter();
    QStringList apply(const QStringList &candidateFilenames, const QStringList& keys, const QStringList& acceptedEndings) const;

};

#endif // FILEFILTER_H
