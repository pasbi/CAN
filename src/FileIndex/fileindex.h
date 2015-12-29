#ifndef BIHASHEDFILEINDEX_H
#define BIHASHEDFILEINDEX_H

#include <QMap>
#include <QCryptographicHash>
#include <QStringList>

class Indexer;
class FileIndex : public QObject
{
    Q_OBJECT

public:
    FileIndex();
    void clear();
    QString filename( const QByteArray & hash ) const;
    QStringList filenames() const;

    /**
     * @brief hash adds the filename to the hash if index does not contain it
     * @param filename
     * @return
     */
    QByteArray hash( const QString & filename );
    bool contains(const QByteArray & hash) { return m_forward.contains(hash); }
    bool contains(const QString & filename) { return m_backward.contains(filename); }

    int size() const { return m_forward.size(); }

    void addDirectory(const QString & path);
    void addFile(const QString& filename);
    QString currentFilename() const;
    bool operationIsFinished() const;

    static QStringList acceptedEndings();

signals:
    void operationFinished();

public slots:
    void abortOperations();

private:
    QMultiMap<QByteArray, QString> m_forward;
    QMap<QString, QByteArray> m_backward;

    const static QCryptographicHash::Algorithm m_hashAlgorithm;

    friend class Indexer;
    void remove(const QString & filename);

    QByteArray serialize() const;
    void deserialize(QByteArray data );

    QStringList m_sources;
    Indexer* m_indexer = nullptr;
    void addFilePrivate(const QString& filename);

private slots:
    void saveDetached() const;
    void save() const;
    void restore();
private:
    mutable bool m_saveRequested = false;
    mutable QThread* m_saverThread = nullptr;
};


#endif // BIHASHEDFILEINDEX_H
