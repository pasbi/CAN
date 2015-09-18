#ifndef LINEEDITDELEGATE_H
#define LINEEDITDELEGATE_H

#include <QLineEdit>

#include "itemdelegate.h"
#include "application.h"
#include "Commands/DatabaseCommands/databaseeditcommand.h"



class LineEditDelegate : public ItemDelegate<QLineEdit>
{
public:
    explicit LineEditDelegate(QObject *parent = 0) :
        ItemDelegate<QLineEdit>(parent)
    {

    }

protected:
    void setEditorData(QLineEdit *editor, const QModelIndex &index) const
    {
        editor->setText( index.model()->data(index, Qt::EditRole).toString() );
    }

    void setModelData(QLineEdit *editor, QAbstractItemModel *model, const QModelIndex &index) const
    {
        QVariant newValue = editor->text();
        if (model->data(index) != newValue)
        {
            app().pushCommand( new DatabaseEditCommand(model, index, newValue ) );
        }
    }
};

#endif // LINEEDITDELEGATE_H
