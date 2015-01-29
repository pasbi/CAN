#include "dir.h"
#include "global.h"

Dir::Dir(Dir *parent, const QString &path) : FileIndexEntry( parent, path )
{
}

bool startsWith(const QString & base, const QString & pattern)
{
    // get top dir form pattern
    QString top = pattern.split("/").first();
    return base.startsWith(top);
}

// top/foo/bar/ -> foo/bar
void removeTopDir( QString & path )
{
    QStringList token = path.split("/");
    token.removeFirst();
    path = token.join("/");
}

FileIndexEntry* Dir::find( QString &filepath )
{
    if (startsWith(filepath, path()))
    {
        removeTopDir(filepath);
        FileIndexEntry* match = NULL;

        for (FileIndexEntry* e : children())
        {
            match = e->find( filepath );
            if (match)
            {
                return match;
            }
        }
        return this;
    }
    else
    {
        return NULL;
    }
}
