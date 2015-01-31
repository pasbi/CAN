#ifndef FILEINDEX_H
#define FILEINDEX_H

#include <QList>
#include <QAbstractTableModel>
#include <QCryptographicHash>
#include "bihashedfileindex.h"


class FileIndex : public QAbstractTableModel
{

public:
    FileIndex();

    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;

    QString entry(const QModelIndex & index) const;
    QModelIndex indexOf(const QString & path) const;

    void addEntry(const QString &absolutePath );
    void addRecursive( const QString & path );

    void setFilter(const QString & filter) { m_filter = filter; }

    void save() const;
    void restore();

public:
    bool removeRows(int row, int count, const QModelIndex &parent);
private:
    bool insertRows(int row, int count, const QModelIndex &parent);


private:
    BiHashedFileIndex m_biHash;
    QString m_filter;
    QList<QString> m_inputBuffer;






};




#endif // FILEINDEX_H
