#include "indexer.h"
#include <QFileInfo>
#include <QDir>
#include "fileindex.h"
#include "global.h"

Indexer::Indexer(const QString &path, const QStringList &acceptedEndings, Mode mode, FileIndex *fileIndex, QObject *parent) :
    QThread(parent),
    m_path(path),
    m_acceptedEndings( acceptedEndings ),
    m_fileIndex(fileIndex),
    m_mode(mode)
{
}

void Indexer::abort()
{
    m_abortFlag = true;
}

void Indexer::run()
{
    switch (m_mode)
    {
    case Update:
        for (const QString & filename : m_fileIndex->m_backward.keys())
        {
            update( filename );
        }
        break;
    case Scan:
        addRecursively( m_path );
        break;
    }
}

void Indexer::update(const QString &path)
{
    QFileInfo info( path );
    if (info.exists() && info.isFile())
    {
        m_fileIndex->addFilePrivate( path );
    }
    else
    {
        m_fileIndex->remove( path );
    }
}

void Indexer::addRecursively(const QString &path)
{
    m_currentFilename = path;
    if (m_abortFlag)
    {
        return;
    }

    if ( QFileInfo( path ).isDir() )
    {
        QDir dir(path);
        QStringList entries = dir.entryList( QDir::NoDotAndDotDot | QDir::Dirs | QDir::Files | QDir::NoSymLinks );
        for ( const QString & entry : entries )
        {

            addRecursively( dir.absoluteFilePath( entry ) );
        }
    }
    else
    {
        QString end = QFileInfo(path).suffix().toLower();
        if (m_acceptedEndings.contains(end))
        {
            m_fileIndex->addFilePrivate( path );
        }
    }
}

QString Indexer::currentFilename() const
{
    return m_currentFilename;
}



