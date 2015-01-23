#ifndef FILEINDEX_H
#define FILEINDEX_H

#include <QString>
#include <QStringList>
#include <QHash>
#include <QByteArray>
#include <QCryptographicHash>


class BiHashedFileIndex
{

public:
    void add(const QString& filename, const QByteArray & hash);
    void remove(const QString & filename);
    void clear();
    QString filename(const QByteArray & hash) const;
    bool contains(const QByteArray & hash) { return m_forward.contains(hash); }
    bool contains(const QString & filename) { return m_backward.contains(filename); }
    QStringList files() const { return m_forward.values(); }

private:
    QHash<QByteArray, QString> m_forward;
    QHash<QString, QByteArray> m_backward;
};

class FileIndex
{
public:
    FileIndex();

    void addToIndex(const QString & rootDirectory);
    void removeFromIndex(const QString & filename);

    static QByteArray hash(const QString& filename);

    QString getFilename(const QByteArray& hash) const;
    QStringList indexedFiles() const;


private:
    BiHashedFileIndex m_index;
    static QStringList listFilesRecursively(const QString & root);

    static QCryptographicHash::Algorithm m_hashAlgorithm;

};

#endif // FILEINDEX_H
