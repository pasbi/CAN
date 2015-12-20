#include "texteditdelegate.h"
#include "Commands/DatabaseCommands/databaseeditcommand.h"
#include "application.h"

TextEditDelegate::TextEditDelegate(QObject *parent) :
    ItemDelegate(parent)
{
}

void TextEditDelegate::setEditorData(QTextEdit* editor, const QModelIndex& index) const
{
    QVariant data = index.model()->data(index, Qt::EditRole);
    editor->setText(data.toString());
    qDebug() << "get data " << data.toString();
}

void TextEditDelegate::setModelData(QTextEdit *editor, QAbstractItemModel *database, const QModelIndex &index) const
{
    qDebug() << "set data " << editor->toPlainText();
    app().pushCommand( new DatabaseEditCommand(database, index, editor->toPlainText()) );
}
