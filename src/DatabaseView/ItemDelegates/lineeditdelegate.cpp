#include "lineeditdelegate.h"

LineEditDelegate::LineEditDelegate(QObject *parent) :
    ItemDelegate(parent)
{

}

void LineEditDelegate::setSpecificEditorData(QLineEdit *editor, const QModelIndex &index) const
{
    editor->setText( index.model()->data(index, Qt::EditRole).toString() );
}

void LineEditDelegate::setSpecificModelData(QLineEdit *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QVariant newValue = editor->text();
    if (model->data(index) != newValue)
    {
        app().pushCommand( new DatabaseEditCommand(model, index, newValue ) );
    }
}
