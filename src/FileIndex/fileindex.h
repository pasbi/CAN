#ifndef FILEINDEX_H
#define FILEINDEX_H

#include <QList>

#include "fileindexentry.h"
#include <QAbstractItemModel>


class FileIndex : public QAbstractItemModel
{
public:
    FileIndex();

    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QModelIndex parent(const QModelIndex &child) const;
    QVariant data(const QModelIndex &index, int role) const;
    QModelIndex index(int row, int column, const QModelIndex &parent) const;

    FileIndexEntry* entry(const QModelIndex & index) const;

    FileIndexEntry* find(QString & path) const;

    void addEntry(const QString &absolutePath );

    QModelIndex indexOf(FileIndexEntry *entry ) const;
    void addRecursive( const QString & path );

private:
    QList<FileIndexEntry*> m_inputBuffer;
    bool insertRows(int row, int count, const QModelIndex &parent);
    void fillGap(const QString & nearest, const QString & current);



private:
    QList<FileIndexEntry*> m_entries;




};




#endif // FILEINDEX_H
