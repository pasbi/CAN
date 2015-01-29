#ifndef DIR_H
#define DIR_H

#include "fileindexentry.h"
#include <QDir>

class Dir : public FileIndexEntry
{
public:
    Dir(Dir *parent, const QString & path);
    FileIndexEntry* find(QString &path);

    bool isDir() const { return true; }

};

#endif // DIR_H
