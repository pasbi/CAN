#include "peoplesdelegate.h"

PeoplesDelegate::PeoplesDelegate(QObject* parent) :
    ItemDelegate(parent)
{
}


void PeoplesDelegate::setEditorData(PeoplesEdit* editor, const QModelIndex& index) const
{

}

void PeoplesDelegate::setModelData(PeoplesEdit *editor, QAbstractItemModel *database, const QModelIndex &index) const
{

}
