#include "editablecomboboxdelegate.h"
#include "itemdelegate.h"
#include <QComboBox>
#include "Commands/DatabaseCommands/databaseeditcommand.h"
#include <QLineEdit>
#include "Database/database.h"

EditableComboBoxDelegate::EditableComboBoxDelegate(QObject* parent) :
    ItemDelegate<QComboBox>(parent)
{

}

void EditableComboBoxDelegate::setSpecificModelData(QComboBox *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    if (editor->currentText() != model->data(index))
    {
        pushCommand( new DatabaseEditCommand(model, index, editor->currentText()) );
    }
}

void EditableComboBoxDelegate::setSpecificEditorData(QComboBox *editor, const QModelIndex &index) const
{
    QStringList columnData;
    int column = index.column();
    for (int row = 0; row < index.model()->rowCount(); ++row )
    {
        QModelIndex cIndex = index.model()->index(row, column);
        columnData << index.model()->data(cIndex, Qt::DisplayRole).toString();
    }
    editor->addItems(columnData);
    editor->setEditable(true);
    editor->setInsertPolicy(QComboBox::NoInsert);
    editor->setCurrentText(index.model()->data(index, Qt::EditRole).toString());
    editor->lineEdit()->selectAll();
}

