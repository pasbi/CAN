#ifndef FILEENTRY_H
#define FILEENTRY_H

#include "fileindexentry.h"
#include "FileIndex/dir.h"

class File : public FileIndexEntry
{
public:
    File(Dir* parent, const QString & path);

    FileIndexEntry* find(QString &filepath);

    bool isDir() const { return false; }
};

#endif // FILEENTRY_H
