#include "keydelegate.h"

KeyDelegate::KeyDelegate(QObject *parent) :
    ItemDelegate(parent)
{
}

void KeyDelegate::setEditorData(KeyEditor* editor, const QModelIndex& index) const
{

}

void KeyDelegate::setModelData(KeyEditor *editor, QAbstractItemModel *database, const QModelIndex &index) const
{

}
