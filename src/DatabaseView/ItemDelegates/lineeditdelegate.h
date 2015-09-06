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
        app().pushCommand( new DatabaseEditCommand(model, index, editor->text() ) );
    }
};

#endif // LINEEDITDELEGATE_H
