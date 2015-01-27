#include "file.h"

File::File(Dir* parent, const QString &relativeFilename) : FileIndexEntry(parent, relativeFilename)
{
}

QString File::absoluteFilename() const
{
    if (parent())
    {
        return parent()->dir().absoluteFilePath(path());
    }
    else
    {
        return path();
    }
}
