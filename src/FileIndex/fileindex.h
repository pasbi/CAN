#ifndef BIHASHEDFILEINDEX_H
#define BIHASHEDFILEINDEX_H

#include <QMap>
#include <QCryptographicHash>
#include <QStringList>
#include "configurable.h"

class Indexer;
class FileIndex : public QObject
{
    Q_OBJECT
    DECL_CONFIG( FileIndex )

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

    void save() const;
    void restore();

    // QStringList.contains is faster than QMap.contains for n < 20. acceptedEndings is expected to be < 10
    void addDirectory(const QString & path, const QStringList& acceptedEndings);
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
    Indexer* m_indexer = NULL;
    void addFilePrivate(const QString& filename);

};


#endif // BIHASHEDFILEINDEX_H
