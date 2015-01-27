#include "fileindex.h"
#include "global.h"
#include <QDir>
#include "file.h"
#include "global.h"


FileIndex::FileIndex()
{
    for (int i = 0; i < 10; ++i)
    {
        m_entries << new File(NULL, QString("/file_%1").arg(i));
        Dir* dir = new Dir(QString("/dir_%1/").arg(i));
        m_entries << dir;
        for (int j = 0; j < 4; ++j)
        {
            new File(dir, QString("subfile_%1").arg(j));
        }
    }
}

int FileIndex::rowCount(const QModelIndex &parent) const
{
    int c = -1;
    if (parent.isValid())
    {
        c = entry(parent)->children().length();
    }
    else
    {
        c = m_entries.length();
    }
    return c;
}

int FileIndex::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 2;   // filename, status
}

QModelIndex FileIndex::parent(const QModelIndex &child) const
{
    FileIndexEntry* e = entry(child);
    if (e->parent())
    {
        int row = e->parent()->children().indexOf(e);
        qDebug() << "parent of " << child << " is " << createIndex(row, 0, e->parent());
        return createIndex(0, 0, e->parent());
    }
    else
    {
        qDebug() << "parent of " << child << " is " << QModelIndex();
        return QModelIndex();
    }
}

FileIndexEntry* FileIndex::entry(const QModelIndex &index) const
{
    return static_cast<FileIndexEntry*>(index.internalPointer());
}


QVariant FileIndex::data(const QModelIndex &index, int role) const
{
    if (role == Qt::DisplayRole)
    {
        switch (index.column())
        {
        case 0:
            return entry(index)->path();
        case 1:
            return "Valid";
        default:
            return QVariant();
        }
    }
    else
    {
        return QVariant();
    }
}

QModelIndex FileIndex::index(int row, int column, const QModelIndex &parent) const
{
    FileIndexEntry* current = 0;
    if (parent.isValid())
    {
        FileIndexEntry* parentEntry = entry(parent);
        current = parentEntry->children()[row];
    }
    else
    {
        current = m_entries[row];
    }
    return createIndex(row, column, current);
}

