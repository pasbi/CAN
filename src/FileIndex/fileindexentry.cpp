#include "fileindexentry.h"
#include "dir.h"
#include "file.h"
#include "global.h"

FileIndexEntry::FileIndexEntry(Dir *parent, const QString &path) :
    m_parent(parent),
    m_path(path)
{
    if (parent)
        parent->m_children.append(static_cast<File*>(this));
}


void FileIndexEntry::insertChild(int i, FileIndexEntry* child)
{
    assert( isDir() );
    m_children.insert(i, child);
    child->m_parent = static_cast<Dir*>(this);
}

FileIndexEntry* FileIndexEntry::create(Dir *parent, const QString &path, const QString & absolutePath)
{
    if (QFileInfo(absolutePath).isDir())
    {
        return new Dir(parent, path);
    }
    else
    {
        return new File(parent, path);
    }
}

QString FileIndexEntry::absolutePath() const
{
    if (isRoot())
    {
        return path();
    }
    else
    {
        return QDir(parent()->absolutePath()).absoluteFilePath(path());
    }
}

