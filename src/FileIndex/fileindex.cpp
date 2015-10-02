#include "fileindex.h"
#include <QFile>
#include "global.h"
#include "indexer.h"
#include <QFileInfo>

DEFN_CONFIG( FileIndex, "File Index" );

CONFIGURABLE_ADD_ITEM( FileIndex,
                       endings,
                       QT_TRANSLATE_NOOP("ConfigurableItem", "Endings"),
                       QT_TRANSLATE_NOOP("ConfigurableItem", "All Endings that may be included"),
                       (QStringList() << "mp3" << "ogg" << "aif" << "aiff" << "pdf").join(QChar(0x2C)), /* 0x2C = , macro does not allow to write it!*/
                       ConfigurableItemOptions::LineEditOptions("") );

const QCryptographicHash::Algorithm FileIndex::m_hashAlgorithm = QCryptographicHash::Sha1;

FileIndex::FileIndex()
{
    restore();
}

void FileIndex::addFilePrivate(const QString& filename)
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

void FileIndex::addFile(const QString& filename)
{
    addFilePrivate(filename);
    saveDetached();
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
    save();
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

QByteArray FileIndex::hash(const QString &filename)
{
    if (!m_backward.contains(filename))
    {
        addFile(filename);
    }
    return m_backward.value( filename );
}

void FileIndex::save() const
{
    m_saveRequested = false;
    QByteArray data;
    QDataStream stream(&data, QIODevice::WriteOnly);

    stream << quint64( m_backward.size() );
    stream << m_sources;

    for (const QString & key : m_backward.keys())
    {
        stream << key << m_backward[key];
    }

    QSettings settings;
    settings.setValue("FileIndex/Index", data);
}


void FileIndex::restore()
{
    QSettings settings;
    QByteArray data = settings.value("FileIndex/Index").toByteArray();

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


void FileIndex::addDirectory(const QString & path)
{
    assert( !m_indexer );
    m_sources << path;
    m_indexer = new Indexer( path,
                             FileIndex::acceptedEndings(),
                             Indexer::Scan,
                             this );
    connect(m_indexer, &QThread::finished, [this]()
    {
        m_indexer->deleteLater();
        m_indexer = nullptr;
        emit operationFinished();
    });
    m_indexer->start();
    connect(m_indexer, SIGNAL(finished()), this, SLOT(saveDetached()));
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
        m_indexer = nullptr;
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

QStringList FileIndex::acceptedEndings()
{
    QStringList endings = config["endings"].toString().split(",");
    for (QString& ending: endings)
    {
        ending = ending.remove(QRegExp("\\W"));
    }
    endings = endings.filter(QRegExp(".+"));
    return endings;
}

void FileIndex::saveDetached() const
{
    class SaverThread : public QThread
    {
    public:
        SaverThread(const FileIndex* index) : m_index(index) {}
        void run() { m_index->save(); }
    private:
        const FileIndex* m_index;
    };

    if (m_saverThread)
    {
        // there is a saver-thread still running. set the requested flag. If the thread terminates, it will run again.
        m_saveRequested = true;
    }
    else
    {
        // there is no saver thread running, so create one.
        m_saverThread = new SaverThread(this);
        connect(m_saverThread, &QThread::finished, [this]()
        {
            // if the thread terminates, delete it and check if a request was made during the run.
            // if a request was made, start again.
            this->m_saverThread->deleteLater();
            this->m_saverThread = nullptr;
            if (this->m_saveRequested)
            {
                this->saveDetached();
            }
        });
        // start the thread.
        m_saverThread->start();
    }
}













