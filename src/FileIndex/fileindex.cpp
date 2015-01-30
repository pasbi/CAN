#include "fileindex.h"
#include "global.h"
#include <QDir>

FileIndex::FileIndex()
{
}

int FileIndex::rowCount(const QModelIndex &parent) const
{
    assert( !parent.isValid() );

    return m_biHash.size();
}

int FileIndex::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 3;   // filename, status, recursive stat, file/dir
}

QString FileIndex::entry(const QModelIndex &index) const
{
    return m_biHash.fileNameAt(index.row());
}


QVariant FileIndex::data(const QModelIndex &index, int role) const
{
    if (role == Qt::DisplayRole)
    {
        switch (index.column())
        {
        case 0:
            return QFileInfo(entry(index)).fileName();
        case 1:
            return QFileInfo(entry(index)).exists() ? tr("Exists") : tr("Does not exist");
        case 2:
            return entry(index);
        }
    }

    return QVariant();
}

bool FileIndex::insertRows(int row, int count, const QModelIndex &parent)
{
    assert(m_inputBuffer.size() == count);
    beginInsertRows(parent, row, row + count - 1);
    for (int i = 0; i < count; ++i)
    {
        m_biHash.add( m_inputBuffer[i] );
    }
    endInsertRows();
    m_inputBuffer.clear();
    return true;
}

bool FileIndex::removeRows(int row, int count, const QModelIndex &parent)
{
    assert( !parent.isValid() );
    beginRemoveRows(QModelIndex(), row, row + count - 1);
    m_biHash.remove(row, count);
    endRemoveRows();
    return true;
}

QModelIndex FileIndex::indexOf(const QString &path) const
{
    int row = m_biHash.indexOf(path);
    return index( row, 0, QModelIndex());
}

void FileIndex::addEntry(const QString & absolutePath)
{
    if ( m_biHash.contains(absolutePath) )
    {
        WARNING << "FileIndex already contains " << absolutePath;
    }
    else
    {
        m_inputBuffer.append(absolutePath);
        insertRows( m_biHash.size(), 1, QModelIndex() );
    }
}

void FileIndex::addRecursive(const QString &path)
{
    if (QFileInfo(path).isFile())
    {
        addEntry(path);
    }
    else
    {
        QDir dir(path);
        for (const QString & entry : dir.entryList( QDir::NoDotAndDotDot | QDir::Hidden | QDir::Files | QDir::Dirs ))
        {
            addRecursive(dir.absoluteFilePath(entry));
        }
    }
}

























