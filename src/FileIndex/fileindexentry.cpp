#include "fileindexentry.h"
#include "dir.h"
#include "file.h"

FileIndexEntry::FileIndexEntry(Dir *parent, const QString &path) :
    m_parent(parent),
    m_path(path)
{
    if (parent)
        parent->m_children.append(static_cast<File*>(this));
}

