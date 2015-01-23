#include "fileindex.h"
#include <QDirIterator>
#include "global.h"

QCryptographicHash::Algorithm FileIndex::m_hashAlgorithm = QCryptographicHash::Sha1;

void BiHashedFileIndex::add(const QString& filename, const QByteArray & hash)
{
    m_forward.insert(hash, filename);
    m_backward.insert(filename, hash);
}

void BiHashedFileIndex::remove(const QString & filename)
{
    QByteArray hash = m_backward.value(filename);
    if (hash.isEmpty())
        return;

    m_backward.remove(filename);
    m_forward.remove(hash);
}

void BiHashedFileIndex::clear()
{
    m_backward.clear();
    m_forward.clear();
}

QString BiHashedFileIndex::filename(const QByteArray &hash) const
{
    return m_forward.value(hash);
}

FileIndex::FileIndex()
{
}

QString FileIndex::getFilename(const QByteArray &hash) const
{
    return m_index.filename(hash);
}

QStringList FileIndex::listFilesRecursively(const QString & root)
{
    QStringList list;
    QDirIterator iterator( QDir(root), QDirIterator::Subdirectories );
    while( iterator.hasNext() )
    {
        list << iterator.next();
    }
    return list;
}

QByteArray FileIndex::hash(const QString &filename)
{
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly))
    {
        qWarning() << "Cannot open " << filename << " for reading.";
        return QByteArray();
    }
    return QCryptographicHash::hash(file.readAll(), m_hashAlgorithm);
}

void FileIndex::addToIndex(const QString &rootDirectory)
{
    const QStringList files = listFilesRecursively( rootDirectory );
    for (const QString & filename : files)
    {
        QByteArray h = hash(filename);
        if (h.isEmpty())
            continue;
        m_index.add( filename, h );

    }
}

void FileIndex::removeFromIndex(const QString &filename)
{
    m_index.remove(filename);
}

QStringList FileIndex::indexedFiles() const
{
    return m_index.files();
}

