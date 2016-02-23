//#include "doubledatabasetreemodel.h"

//DoubleDatabaseTreeModel::DoubleDatabaseTreeModel(SongDatabase* database) :
//    m_database(database)
//{
//}

//QModelIndex DoubleDatabaseTreeModel::index(int row, int column, const QModelIndex &parent) const
//{

//}

//QModelIndex DoubleDatabaseTreeModel::parent(const QModelIndex &child) const
//{

//}

//int DoubleDatabaseTreeModel::rowCount(const QModelIndex &parent) const
//{

//}

//int DoubleDatabaseTreeModel::columnCount(const QModelIndex &parent) const
//{
//    Q_UNUSED(parent);
//    return 1;
//}

//QVariant DoubleDatabaseTreeModel::data(const QModelIndex &index, int role) const
//{

//}

//Song* DoubleDatabaseTreeModel::songAt(const QModelIndex &index) const
//{
//    if (index.parent().isValid())
//    {
//        return nullptr;
//    }
//    else
//    {
//        return m_database->itemAtIndex(index);
//    }
//}

