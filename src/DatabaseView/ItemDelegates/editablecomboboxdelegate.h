#ifndef EDITABLECOMBOBOXDELEGATE_H
#define EDITABLECOMBOBOXDELEGATE_H

#include "itemdelegate.h"
#include <QComboBox>
#include "Commands/DatabaseCommands/databaseedititemcommand.h"
#include "application.h"
#include <QLineEdit>

template<typename T>
class EditableComboBoxDelegate : public ItemDelegate<T, QComboBox>
{
public:
    EditableComboBoxDelegate(QObject* parent = 0) :
        ItemDelegate<T, QComboBox>(parent)
    {

    }

private:
    void setModelData(QComboBox *editor, Database<T> *database, const QModelIndex &index) const
    {
        app().pushCommand( new DatabaseEditItemCommand<T>(database, index, editor->currentText()) );
    }

    void setEditorData(QComboBox *editor, const QModelIndex &index) const
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
};

#endif // EDITABLECOMBOBOXDELEGATE_H
