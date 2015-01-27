#ifndef FILEENTRY_H
#define FILEENTRY_H

#include "fileindexentry.h"
#include "FileIndex/dir.h"

class File : public FileIndexEntry
{
public:
    File(Dir* parent, const QString & relativeFilename);

    QString absoluteFilename() const;
};

#endif // FILEENTRY_H
