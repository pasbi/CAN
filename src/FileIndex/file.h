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

    void updateHash();

    QByteArray hash() const { return m_hash; }

private:
    QByteArray m_hash;
};

#endif // FILEENTRY_H
