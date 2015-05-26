#ifndef BIHASHEDFILEINDEX_H
#define BIHASHEDFILEINDEX_H

#include <QMap>
#include <QCryptographicHash>
#include <QStringList>
#include <QSettings>
#include "indexer.h"
#include "configurable.h"

class FileIndex : public QObject
{
    Q_OBJECT
    DECL_CONFIG( FileIndex )

public:
    FileIndex();
    void clear();
    QString filename( const QByteArray & hash ) const;
    QStringList filenames() const;
    QByteArray hash( const QString & filename ) const;
    bool contains(const QByteArray & hash) { return m_forward.contains(hash); }
    bool contains(const QString & filename) { return m_backward.contains(filename); }

    int size() const { return m_forward.size(); }

    void save() const;
    void restore();

    void addSource(const QString & path, const QStringList &filter = QStringList() << "*");
    QString currentFilename() const;
    bool operationIsFinished() const;

signals:
    void operationFinished();

public slots:
    void abortOperations();

private:
    QMultiMap<QByteArray, QString> m_forward;
    QMap<QString, QByteArray> m_backward;

    const static QCryptographicHash::Algorithm m_hashAlgorithm;

    friend class Indexer;
    void add(const QString& filename);
    void remove(const QString & filename);

    QByteArray serialize() const;
    void deserialize(QByteArray data );

    QStringList m_sources;
    Indexer* m_indexer = NULL;

};


#endif // BIHASHEDFILEINDEX_H
