#include "bihashedfileindex.h"
#include <QFile>

QCryptographicHash::Algorithm BiHashedFileIndex::m_hashAlgorithm = QCryptographicHash::Sha1;

void BiHashedFileIndex::add(const QString& filename)
{
    QFile file(filename);
    QByteArray hash;

    if (file.open(QIODevice::ReadOnly))
    {

        hash = QCryptographicHash::hash( file.readAll(),
                                           QCryptographicHash::Sha3_512 );
    }
    else
    {
        hash.clear();
    }


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

void BiHashedFileIndex::remove( int i, int n )
{
    QStringList keys;
    for (int k = i; k < n; ++k)
    {
        keys << fileNameAt(k);
    }

    for (const QString & key : keys)
    {
        remove(key);
    }
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
