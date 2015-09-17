#ifndef INDEXER_H
#define INDEXER_H

#include <QThread>
#include <QMap>

class FileIndex;
class Indexer : public QThread
{
    Q_OBJECT
public:
    enum Mode { Update, Scan };
    explicit Indexer(const QString & path, const QStringList& acceptedEndings,
                     Mode mode, FileIndex* fileIndex, QObject *parent = 0);

    void abort();
    QString currentFilename() const;

protected:
    void run();

private:

    const QString m_path;
    const QStringList& m_acceptedEndings;
    FileIndex* const m_fileIndex;
    const Mode m_mode;

    bool m_abortFlag = false;

    void addRecursively( const QString & path );
    void update( const QString & path );

    mutable QString m_currentFilename;

};

#endif // INDEXER_H
