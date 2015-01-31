#ifndef BIHASHEDFILEINDEX_H
#define BIHASHEDFILEINDEX_H

#include <QMap>
#include <QCryptographicHash>
#include <QStringList>
#include <QSettings>


class BiHashedFileIndex
{

public:
    void add(const QString& filename);
    void remove(const QString & filename);
    void remove(int i , int n);
    void clear();
    QString filename(const QByteArray & hash) const;
    bool contains(const QByteArray & hash) { return m_forward.contains(hash); }
    bool contains(const QString & filename) { return m_backward.contains(filename); }
    QStringList files() const { return m_files; }


    int size() const { return m_forward.size(); }
    QString fileNameAt(int index) const { return m_files[index]; }
    int indexOf(const QString & path) const { return m_files.indexOf( path ); }

    void save( QSettings & settings ) const;
    void restore( const QSettings & settings );

private:
    QMap<QByteArray, QString> m_forward;
    QMap<QString, QByteArray> m_backward;
    QStringList m_files;
    static QCryptographicHash::Algorithm m_hashAlgorithm;

};


#endif // BIHASHEDFILEINDEX_H
