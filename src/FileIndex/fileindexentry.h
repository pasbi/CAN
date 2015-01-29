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
    QString absolutePath() const;
    QList<FileIndexEntry*> children() const { return m_children; }

    bool isRoot() const { return !parent(); }

    virtual FileIndexEntry* find(QString & path) = 0;
    void insertChild(int i, FileIndexEntry* child);
    virtual bool isDir() const = 0;
    bool isFile() const { return !isDir(); }

    /**
     * @brief create creates an directory or file entry.
     * @param parent
     * @param path
     * @return
     */
    static FileIndexEntry* create(Dir* parent, const QString & path, const QString &absolutePath);


private:

    Dir* m_parent;
    const QString m_path;
    QList<FileIndexEntry*> m_children;
};

#endif // FILEINDEXENTRY_H
