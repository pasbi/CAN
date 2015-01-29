#include "file.h"

File::File(Dir* parent, const QString &path) : FileIndexEntry(parent, path)
{
}

FileIndexEntry* File::find(QString &filepath)
{
    if (filepath == path())
    {
        filepath = "";
        return this;
    }
    else
    {
        return NULL;
    }
}
