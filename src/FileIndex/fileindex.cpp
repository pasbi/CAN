#include "fileindex.h"
#include <QFile>
#include "global.h"
#include "indexer.h"
#include "progressdialog.h"
#include <QFileInfo>

REGISTER_DEFN_CONFIG( FileIndex, "File Index" );

CONFIGURABLE_ADD_ITEM( FileIndex, FileIndex, QByteArray(), ConfigurationItemOptions::HiddenInterface() );

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
    }

    if (m_backward.contains(filename))
    {
        remove( filename );
    }

    m_forward.insert(hash, filename);
    m_backward.insert(filename, hash);
}

void FileIndex::remove(const QString & filename)
{
    QByteArray hash = m_backward.value(filename);

    m_backward.remove(filename);
    m_forward.remove(hash);
}

void FileIndex::clear()
{
    m_backward.clear();
    m_forward.clear();
    m_sources.clear();
}

QString FileIndex::filename(const QByteArray &hash) const
{
    return m_forward.value(hash);
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
    config.setItem( "FileIndex", serialize() );
}

void FileIndex::restore( )
{
    deserialize( config.item("FileIndex").toByteArray() );
}

Indexer* FileIndex::requestIndexer( const QString & path, const QStringList filter, Indexer::Mode mode )
{
    if ( m_indexer )
    {
        WARNING << "Wait for current operation to finish.";
        return NULL;
    }
    else
    {
        m_indexer = new Indexer( path, filter, mode, this, NULL );
        m_indexer->connect( m_indexer, &QThread::finished, [this]()
        {
            m_indexer->deleteLater();
            m_indexer = 0;
        });
        m_indexer->start();
        return m_indexer;
    }
}

void FileIndex::abortIndexing()
{
    if ( m_indexer )
    {
        m_indexer->abort();
    }
    else
    {
        WARNING << "There is no operation to abort.";
    }
}

void FileIndex::addSource( const QString & path, const QStringList & filter )
{
    m_sources << path;
    requestIndexer( path, filter, Indexer::Scan )->start();
}


void FileIndex::updateIndex()
{
    for (const QString & source : m_sources)
    {
        requestIndexer( source, QStringList(), Indexer::Update );
    }
}

QStringList FileIndex::filenames( ) const
{
    return m_backward.keys();
}















