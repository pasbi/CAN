#include "combinedialogselectionmodel.h"
#include "global.h"

CombineDialogSelectionModel::CombineDialogSelectionModel(QAbstractItemModel *parent) :
    QItemSelectionModel(parent)
{
}

void CombineDialogSelectionModel::select(const QModelIndex &index, SelectionFlags command)
{
    QItemSelectionModel::select(QItemSelection(index, index), command);
}

void CombineDialogSelectionModel::select(const QItemSelection &selection, QItemSelectionModel::SelectionFlags command)
{
    qDebug() << "command = " << command << Select;
    if (command & Select)
    {
        for (const QModelIndex& selected : selection.indexes())
        {
            for (int column = 0; column < model()->columnCount(); ++column)
            {
                QModelIndex index = model()->index(selected.row(), column);
                qDebug() << "index: " << index << isSelected(index) << (index == selected);
                if (isSelected(index) && index != selected)
                {
                    qDebug() << "  deselect!";
                    select(index, QItemSelectionModel::Deselect);
                }
                else if (!isSelected(index) && index == selected)
                {
                    qDebug() << "  select!";
                    select(index, QItemSelectionModel::Select);
                }
            }
        }
    }
}

