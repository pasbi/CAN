#ifndef LINEEDITDELEGATE_H
#define LINEEDITDELEGATE_H

#include <QLineEdit>

#include "itemdelegate.h"
#include "application.h"
#include "Commands/DatabaseCommands/databaseedititemcommand.h"

template<typename T>
class LineEditDelegate : public ItemDelegate<T, QLineEdit>
{
public:
    explicit LineEditDelegate(QObject *parent = 0) :
        ItemDelegate<T, QLineEdit>(parent)
    {

    }

protected:
    void setEditorData(QLineEdit *editor, const QModelIndex &index) const
    {
        editor->setText( index.model()->data(index, Qt::EditRole).toString() );
    }

    void setModelData(QLineEdit *editor, Database<T> *database, const QModelIndex &index) const
    {
        app().pushCommand( new DatabaseEditItemCommand<T>(database, index, editor->text() ) );
    }
};

#endif // LINEEDITDELEGATE_H
