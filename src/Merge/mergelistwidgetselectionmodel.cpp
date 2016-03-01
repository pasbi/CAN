#include "mergelistwidgetselectionmodel.h"
#include "global.h"

MergeListWidgetSelectionModel::MergeListWidgetSelectionModel(QAbstractItemModel *parent) :
    QItemSelectionModel(parent)
{
}

void MergeListWidgetSelectionModel::select(const QModelIndex &index, SelectionFlags command)
{
    QItemSelectionModel::select(QItemSelection(index, index), command);
}

void MergeListWidgetSelectionModel::select(const QItemSelection &selection, QItemSelectionModel::SelectionFlags command)
{
    if (selection.isEmpty())
    {
        return;
    }

    QModelIndex toBeSelected = selection.last().bottomRight();
    QItemSelection deselect;
    QItemSelection select;
    if (command & Deselect)
    {
        deselect << selection;
    }
    else if (command & Select)
    {
        for (int row = 0; row < model()->rowCount(); ++row)
        {
            QModelIndex index = model()->index(row, 0);
            if (index == toBeSelected)
            {
                select << QItemSelectionRange(index);
            }
            else
            {
                deselect << QItemSelectionRange(index);
            }
        }
    }

    QItemSelectionModel::select(select, QItemSelectionModel::Select);
    QItemSelectionModel::select(deselect, QItemSelectionModel::Deselect);
}

