#ifndef MERGELISTWIDGETSELECTIONMODEL_H
#define MERGELISTWIDGETSELECTIONMODEL_H

#include <QItemSelectionModel>

class MergeListWidgetSelectionModel : public QItemSelectionModel
{
    Q_OBJECT
public:
    explicit MergeListWidgetSelectionModel(QAbstractItemModel *parent = 0);

    void select(const QModelIndex &index, SelectionFlags command);
    void select(const QItemSelection &selection, QItemSelectionModel::SelectionFlags command);

};

#endif // MERGELISTWIDGETSELECTIONMODEL_H
