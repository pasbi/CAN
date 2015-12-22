#include "peoplesdelegate.h"

PeoplesDelegate::PeoplesDelegate(QObject* parent) :
    ItemDelegate(parent)
{
}


void PeoplesDelegate::setSpecificEditorData(PeoplesEdit* editor, const QModelIndex& index) const
{

}

void PeoplesDelegate::setSpecificModelData(PeoplesEdit *editor, QAbstractItemModel *database, const QModelIndex &index) const
{

}
