#include "fileindex.h"
#include <QFile>
#include "global.h"
#include "indexer.h"
#include <QFileInfo>

DEFN_CONFIG( FileIndex, "File Index" );

CONFIGURABLE_ADD_ITEM_HIDDEN( FileIndex, FileIndex, QByteArray() );

const QCryptographicHash::Algorithm FileIndex::m_hashAlgorithm = QCryptographicHash::Sha1;

FileIndex::FileIndex()
{

}

void FileIndex::add(const QString& filename)
{
    QFile file(filename);
    QByteArray hash;

    if (file.open(QIODevice::ReadOnly))
    {

        const quint64 BUFFER_SIZE = 16000;  // reading the whole file is to slow. guess the first 16k bytes should be distinct.
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
        return;
    }

    if (m_backward.contains(filename))
    {
        remove( filename );
    }

    // one hash can have multiple filenames.
    // the files then are exact copies of each others.
    // but one filename must have one single hash.
    m_forward.insert(hash, filename);
    m_backward.insert(filename, hash);
}

void FileIndex::remove(const QString & filename)
{
    QByteArray hash = m_backward.value(filename);

    m_backward.remove(filename);
    m_forward.remove(hash, filename);
}

void FileIndex::clear()
{
    m_backward.clear();
    m_forward.clear();
    m_sources.clear();
}

QString FileIndex::filename(const QByteArray &hash) const
{
    // find the first filename associated to that hash which is readable.
    QStringList filenames = m_forward.values( hash );
    for (const QString & filename : filenames)
    {
        if (QFileInfo(filename).isReadable())
        {
            return filename;
        }
    }
    return filenames.isEmpty() ? QString() : filenames.first();
}

QByteArray FileIndex::hash(const QString &filename) const
{
    return m_backward.value( filename );
}

QByteArray FileIndex::serialize() const
{
    QByteArray data;
    QDataStream stream(&data, QIODevice::WriteOnly);

    stream << quint64( m_backward.size() );
    stream << m_sources;

    for (const QString & key : m_backward.keys())
    {
        stream << key << m_backward[key];
    }

    return data;
}


void FileIndex::deserialize( QByteArray data )
{
    QDataStream stream(&data, QIODevice::ReadOnly);
    quint64 size;
    stream >> size;
    stream >> m_sources;

    m_backward.clear();
    m_forward.clear();
    for (quint64 i = 0; i < size; ++i)
    {
        QString filename;
        QByteArray hash;
        stream >> filename >> hash;
        m_backward.insert(filename, hash);
        m_forward.insert(hash, filename);
    }
}


void FileIndex::save( ) const
{
    config.set( "FileIndex", serialize() );
}

void FileIndex::restore( )
{
    deserialize( config.value("FileIndex").toByteArray() );
}


void FileIndex::addSource(const QString & path, bool pdf, bool mp3, bool ogg , bool aif)
{
    assert( !m_indexer );
    m_sources << path;
    m_indexer = new Indexer( path, pdf, mp3, ogg, aif, Indexer::Scan, this );
    connect(m_indexer, &QThread::finished, [this]()
    {
        m_indexer->deleteLater();
        m_indexer = NULL;
        emit operationFinished();
    });
    m_indexer->start();

}

bool FileIndex::operationIsFinished() const
{
    if (m_indexer)
    {
        return m_indexer->isFinished();
    }
    else
    {
        return true;
    }
}


QStringList FileIndex::filenames( ) const
{
    return m_backward.keys();
}

void FileIndex::abortOperations()
{
    if ( m_indexer )
    {
        m_indexer->abort();
        m_indexer->wait();
        m_indexer->deleteLater();
        m_indexer = NULL;
    }
}

QString FileIndex::currentFilename() const
{
    if (m_indexer)
    {
        return m_indexer->currentFilename();
    }
    else
    {
        qWarning() << "expected to have an m_indexer.";
        return "";
    }
}















