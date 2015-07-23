#ifndef INDEXER_H
#define INDEXER_H

#include <QThread>
#include <QStringList>

class FileIndex;
class Indexer : public QThread
{
    Q_OBJECT
public:
    enum Mode { Update, Scan };
    explicit Indexer(const QString & path,
                     bool pdf,
                     bool mp3,
                     bool ogg,
                     bool aif,
                     Mode mode, FileIndex* fileIndex, QObject *parent = 0);

    void abort();
    QString currentFilename() const;

protected:
    void run();

private:

    const QString m_path;
    const bool m_pdf, m_mp3, m_ogg, m_aif;
    FileIndex* const m_fileIndex;
    const Mode m_mode;

    bool m_abortFlag = false;

    void addRecursively( const QString & path );
    void update( const QString & path );

    mutable QString m_currentFilename;

};

#endif // INDEXER_H
