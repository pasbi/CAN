#include "bihashedfileindex.h"
#include <QFile>
#include "global.h"


QCryptographicHash::Algorithm BiHashedFileIndex::m_hashAlgorithm = QCryptographicHash::Sha1;

void BiHashedFileIndex::add(const QString& filename)
{
    QFile file(filename);
    QByteArray hash;

    if (file.open(QIODevice::ReadOnly))
    {

        const quint64 BUFFER_SIZE = 16000;
        QByteArray buffer;
        buffer.resize(BUFFER_SIZE);
        buffer.fill( 0 );

        file.read( buffer.data(), BUFFER_SIZE  );
        hash = QCryptographicHash::hash( buffer,
                                         m_hashAlgorithm );

    }
    else
    {
        hash.clear();
    }


    m_forward.insert(hash, filename);
    m_backward.insert(filename, hash);
    m_files.append(filename);
}

void BiHashedFileIndex::remove(const QString & filename)
{
    QByteArray hash = m_backward.value(filename);
    if (hash.isEmpty())
        return;

    m_backward.remove(filename);
    m_forward.remove(hash);
    m_files.removeOne(filename);
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


void BiHashedFileIndex::save( QSettings & settings ) const
{
    QStringList hashes;
    for (const QString & file : m_files)
    {
        hashes.append( QString::fromLatin1(m_backward[file].toHex()) );
    }
    settings.setValue("files",  m_files);
    settings.setValue("hashes", hashes );
}

void BiHashedFileIndex::restore( const QSettings & settings )
{
    m_files.clear();
    m_forward.clear();
    m_backward.clear();

    QStringList hashes;
    m_files = settings.value("files").toStringList();
    hashes  = settings.value("hashes").toStringList();

    for ( int i = 0; i < m_files.length(); ++i )
    {
        QByteArray hash = QByteArray::fromHex( hashes[i].toLatin1() );
        m_forward.insert(  hash,  m_files[i] );
        m_backward.insert( m_files[i], hash  );
    }
}














