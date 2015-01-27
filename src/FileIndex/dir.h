#ifndef DIR_H
#define DIR_H

#include "fileindexentry.h"
#include <QDir>

class Dir : public FileIndexEntry
{
public:
    // is always root
    Dir(const QString & absolutePath);

    QDir dir() const { return QDir(path()); }

};

#endif // DIR_H
