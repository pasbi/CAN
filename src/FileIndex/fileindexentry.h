#ifndef FILEINDEXENTRY_H
#define FILEINDEXENTRY_H

#include <QString>
#include <QList>

class Dir;
class File;
class FileIndexEntry
{
protected:
    // an parent is always NULL or Dir*
    FileIndexEntry(Dir* parent, const QString & path);

public:
    Dir* parent() const { return m_parent; }

    /**
     * @brief path returns the absolute path to dir path to file, relative to parent dir.
     * @return
     */
    QString path() const { return m_path; }
    QList<FileIndexEntry*> children() const { return m_children; }

    bool isRoot() const { return !!parent(); }



private:
    // parent is always a dir, children is always a file.
    // thus, files may have parent, dirs never have.
    Dir* const m_parent;
    const QString m_path;
    QList<FileIndexEntry*> m_children;
};

#endif // FILEINDEXENTRY_H
