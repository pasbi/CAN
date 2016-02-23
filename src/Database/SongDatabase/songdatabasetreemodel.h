//#ifndef SONGDATABASETREEMODEL_H
//#define SONGDATABASETREEMODEL_H

//#include <QAbstractItemModel>
//#include "songdatabase.h"

///**
// * @brief The SongDatabaseTreeModel class is a wrapper of the database to a tree model.
// */
//class SongDatabaseTreeModel : public QAbstractItemModel
//{
//    Q_OBJECT
//public:
//    explicit SongDatabaseTreeModel(SongDatabase* database);

//    QModelIndex index(int row, int column, const QModelIndex &parent) const;
//    QModelIndex parent(const QModelIndex &child) const;
//    int rowCount(const QModelIndex &parent) const;
//    int columnCount(const QModelIndex &parent) const;
//    QVariant data(const QModelIndex &index, int role) const;
//    Song* songAt(const QModelIndex& index) const;
//    Attachment* attachmentAt(const QModelIndex& index) const;

//private:
//    SongDatabase* m_database;

//};

//#endif // SONGDATABASETREEMODEL_H
